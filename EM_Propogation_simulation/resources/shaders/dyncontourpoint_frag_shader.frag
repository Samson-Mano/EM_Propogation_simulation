#version 330 core

in float v_defl_length;
in float v_transparency;

out vec4 f_Color; // fragment's final color (out to the fragment shader)


vec3 jetHeatmap(float value) 
{

    return clamp(vec3(1.5) - abs(4.0 * vec3(value) + vec3(-3, -2, -1)), vec3(0), vec3(1));
}


void main() 
{
    // Get the input transparency
    float transparency = v_transparency;
 
    // Calculate vertex color using jetHeatmap
    vec3 vertexColor = jetHeatmap(v_defl_length);

    // Set the final color
    f_Color = vec4(vertexColor, transparency); // Set the final color
}
