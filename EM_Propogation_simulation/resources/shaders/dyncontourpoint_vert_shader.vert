#version 330 core

uniform mat4 modelMatrix;
uniform mat4 panTranslation;
uniform float zoomscale;

uniform float normalized_deflscale; // Sine cycle from animation (-1 to 1)
uniform float deflscale; // Deflection scale value = normalized_deflscale (varies 0 to 1) * max deformation
uniform float transparency = 1.0f;
uniform float geom_scale;

layout(location = 0) in vec2 tri_pt;
layout(location = 1) in vec3 tri_pt_color;

out vec4 v_Color;

void main()
{
	// apply zoom scaling and Rotation to model matrix
	mat4 scalingMatrix = mat4(1.0)*zoomscale;
	scalingMatrix[3][3] = 1.0f;
	mat4 scaledModelMatrix = scalingMatrix * modelMatrix;
	
	// Declare variable to store final node center
	vec4 finalPosition;
	vec3 final_vertexColor;

	// apply Transformation to the triangle point position 
	finalPosition = scaledModelMatrix * vec4(tri_pt,0.0f,1.0f) * panTranslation;

	// Vertex color
	final_vertexColor = vec3((0.5f*(1.0f-normalized_deflscale)+(tri_pt_color.x*normalized_deflscale)),
							 (0.0f*(1.0f-normalized_deflscale)+(tri_pt_color.y*normalized_deflscale)),
							 (1.0f*(1.0f-normalized_deflscale)+(tri_pt_color.z*normalized_deflscale)));
	

	v_Color = vec4(final_vertexColor,transparency);

	// Final position passed to fragment shader
	gl_Position = finalPosition;
}