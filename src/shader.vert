#version 460 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_tex_coord;

out vec2 f_tex_coord;

void main()
{
    f_tex_coord = in_tex_coord;
    gl_Position = vec4(in_position, 1.0);
}
