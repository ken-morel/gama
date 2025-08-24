magick gama.ico -colorspace Gray \
    \( -size 50x50 xc:none -fill "rgba(220, 50, 50, 0.9)" -strokewidth 9 \
    -draw "line 8,8 42,42" -draw "line 8,42 42,8" \) \
    -gravity southeast -composite ungama.ico
