function angle_mod = ModAngle(angle)

angle_mod = mod(angle, 360);

if (angle_mod > 180)
    angle_mod = angle_mod - 360;
elseif (angle_mod < -180)
    angle_mod = angle_mod + 360;
end

end