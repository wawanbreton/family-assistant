#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D source;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    vec2 target_size;
    int border_width;
    float blur_factor;
    vec4 color;
};

void main()
{
    int nb_samples = 0;
    int diff_in_out = 0;
    vec2 my_size = target_size + vec2(2.0 * border_width);
    vec2 src_coord = ((qt_TexCoord0 * my_size) - vec2(border_width)) / target_size;

    for(int x = -border_width ; x <= border_width ; ++x)
    {
        for(int y = -border_width ; y <= border_width ; ++y)
        {
            if(length(vec2(x,y)) <= border_width)
            {
                nb_samples++;
                vec2 texCoord = src_coord + vec2(x, y) / target_size;
                if(texCoord.x < 0.0 || texCoord.x > 1.0 || texCoord.y < 0.0 || texCoord.y > 1.0)
                {
                    diff_in_out--;
                }
                else
                {
                    vec4 col = texture(source, texCoord);
                    if(col.r + col.g + col.b + col.a > 0.01)
                    {
                        diff_in_out++;
                    }
                    else
                    {
                        diff_in_out--;
                    }
                }
            }
        }
    }

    float opacity;

    if(diff_in_out >= 0)
    {
        // Pixel is inside
        opacity = 1.0;
    }
    else if(abs(diff_in_out) == nb_samples)
    {
        // Pixel is outside
        discard;
    }
    else
    {
        // pixel is on the outer border
        opacity = pow(((nb_samples + diff_in_out) / float(nb_samples)), blur_factor);
    }

    // Qt rendering stack expects pre-multiplied alpha
    fragColor = vec4(color.rgb, 1.0) * opacity * qt_Opacity;
}
