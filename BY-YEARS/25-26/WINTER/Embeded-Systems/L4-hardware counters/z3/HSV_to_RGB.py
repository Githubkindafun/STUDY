import colorsys

for H in range(1, 361):
    h = H / 360.0
    s = 1.0
    v = 1.0
    r, g, b = colorsys.hsv_to_rgb(h, s, v)
    
    R = int(round(255 * r))
    G = int(round(255 * g))
    B = int(round(255 * b))
    print(f"{{ {R}, {G}, {B} }},")
