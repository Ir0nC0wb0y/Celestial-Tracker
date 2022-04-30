
clear
close all
clc

url = 'https://api.wheretheiss.at/v1/satellites/25544?units=miles';
options=weboptions; options.CertificateFilename=('');

mi2ft = 5280;
ft2mi = 1 / 5280;

updateRate = 1; % hz

trkLat = 30.481369772126065;
trkLon = -86.41068323968271;
trkAlt = 0;
trkPos = [trkLat, trkLon, trkAlt];

blueLen  = 75;
greenLen = 10;
redLen   = 40;

azTrk = 0;
elTrk = 0;
trkDir = [azTrk, elTrk];

p1 = [0; 0; 0];
p2 = p1 + [0; 0; blueLen];
p3 = p2 + greenLen*[-cosd(azTrk); sind(azTrk); 0];
p4 = p3 + redLen*[sind(azTrk)*cosd(elTrk); cosd(azTrk)*cosd(elTrk); sind(elTrk)];

figure(1)
hold on 
grid on
ph(2) = plot3([p2(1) p3(1)], [p2(2) p3(2)], [p2(3) p3(3)], 'go-', 'linewidth', 10);
ph(1) = plot3([p1(1) p2(1)], [p1(2) p2(2)], [p1(3) p2(3)], 'bo-', 'linewidth', 10);
ph(3) = plot3([p3(1) p4(1)], [p3(2) p4(2)], [p3(3) p4(3)], 'ro-', 'linewidth', 10);
view(3)
xlabel('east [mm]')
ylabel('north [mm]')
zlabel('up [mm]')
title('Celestial Tracker | ISS')
axis(75*[-1 1 -1 1 0 2])
axis square
shg

deltaTime = 1 / updateRate;

while(true)
    
    issApi = getCelestialData(url);
    
    satLat = issApi.latitude;
    satLon = issApi.longitude;
    satAlt = issApi.altitude * mi2ft;
    satPos = [satLat, satLon, satAlt];
    
    satDir = getSatDirection(satPos, trkPos);
    
    cmdDir = getDirectionCommand(satDir, trkDir);
    trkDir = cmdDir;
    
    fprintf('Lat: %9.4f deg,  Lon: %9.4f deg,  Alt: %7.2f mi,  Az: %7.2f deg,  El: %7.2f deg\n',...
        satLat, satLon, satAlt*ft2mi, satDir(1), satDir(2));
    
    azTrk = trkDir(1);
    elTrk = trkDir(2);
    
    p1 = [0; 0; 0];
    p2 = p1 + [0; 0; blueLen];
    p3 = p2 + greenLen*[-cosd(azTrk); sind(azTrk); 0];
    p4 = p3 + redLen*[sind(azTrk)*cosd(elTrk); cosd(azTrk)*cosd(elTrk); sind(elTrk)];
    
    ph(1).XData = [p1(1) p2(1)];
    ph(1).YData = [p1(2) p2(2)];
    ph(1).ZData = [p1(3) p2(3)];
    
    ph(2).XData = [p2(1) p3(1)];
    ph(2).YData = [p2(2) p3(2)];
    ph(2).ZData = [p2(3) p3(3)];
    
    ph(3).XData = [p3(1) p4(1)];
    ph(3).YData = [p3(2) p4(2)];
    ph(3).ZData = [p3(3) p4(3)];
    
    shg
    
    pause(deltaTime);
    
end

function api = getCelestialData(url)

options=weboptions; options.CertificateFilename=(''); 
api = webread(url, options);

end

function cmdDir = getDirectionCommand(satDir, trkDir)

azSat = satDir(1);
elSat = satDir(2);

azTrk = trkDir(1);
elTrk = trkDir(2);

uSat = [sind(azSat)*cosd(elSat); cosd(azSat)*cosd(elSat); sind(elSat)];

az1 = atan2d(uSat(1), uSat(2));
el1 = asind(uSat(3));

az2 = mod(az1 + 180, 360);
el2 = mod(180 - el1, 360);

cost1 = abs(az1 - azTrk) + abs(el1 - elTrk);
cost2 = abs(az2 - azTrk) + abs(el2 - elTrk);

if (cost1 < cost2)
    azCmd = az1;
    elCmd = el1;
else
    azCmd = az2;
    elCmd = el2;
end

cmdDir = [azCmd, elCmd];

end

function satDir = getSatDirection(satPos, trkPos)

satLat = satPos(1);
satLon = satPos(2);
satAlt = satPos(3);

trkLat = trkPos(1);
trkLon = trkPos(2);
trkAlt = trkPos(3);

[satX, satY, satZ] = lla2ecef(satLat, satLon, satAlt);
[trkX, trkY, trkZ] = lla2ecef(trkLat, trkLon, trkAlt);

relPosEcef = [(satX - trkX); (satY - trkY); (satZ - trkZ)];

dcmEcefToEnu = ecef2enu(trkLat, trkLon);

relPosEnu = dcmEcefToEnu * relPosEcef;

relPosEnuUnit = relPosEnu / norm(relPosEnu);

azSat = atan2d(relPosEnuUnit(1), relPosEnuUnit(2));
elSat = atand(relPosEnuUnit(3) / norm(relPosEnuUnit(1:2)));

satDir = [azSat, elSat];

end

function dcm = ecef2enu(lat, lon)

slat = sind(lat);
slon = sind(lon);

clat = cosd(lat);
clon = cosd(lon);

dcm(1, 1) = -slon;
dcm(2, 1) = -slat * clon;
dcm(3, 1) =  clat * clon;

dcm(1, 2) =  clon;
dcm(2, 2) = -slat * slon;
dcm(3, 2) =  clat * slon;

dcm(1, 3) =  0;
dcm(2, 3) =  clat;
dcm(3, 3) =  slat;

end

function [x, y, z] = lla2ecef(lat, lon, alt)

% WGS-84 Constants 
radius = 20925646.32537562; % earth major radius [ft]
ellip  = 1/298.257223563;   % earth ellipticity
ecc_2  = ellip*(2 - ellip); % earth eccentricity squared

% Radii of Curvature
R0 = radius / sqrt(1 - ecc_2*sind(lat)^2);  % normal radius of curvature
R1 = R0 + alt;
R2 = R0 * (1 - ecc_2) + alt;                % meridian radius of curvature

% ECEF Coordinates
x = -R1 * (-cosd(lon) * cosd(lat));
y = -R1 * (-sind(lon) * cosd(lat));
z = -R2 * (-sind(lat));

end
