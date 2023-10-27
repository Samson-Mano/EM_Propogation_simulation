#version 330 core

uniform mat4 modelMatrix;
uniform mat4 panTranslation;
uniform float zoomscale;

uniform float normalized_deflscale; // Sine cycle from animation (-1 to 1)
uniform float deflscale; // Deflection scale value = normalized_deflscale (varies 0 to 1) * max deformation
uniform float transparency = 1.0f;
uniform float geom_scale;

layout(location = 0) in vec2 vector_origin;
layout(location = 1) in vec2 pt_position;
layout(location = 2) in vec3 vertexColor;
layout(location = 3) in float pt_type;

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

	if(pt_type == 0.0f)
	{
		// pt_type 0 is origin point
		// apply Transformation to the vector origin position 
		finalPosition = scaledModelMatrix * vec4(vector_origin,0.0f,1.0f) * panTranslation;

		// Vertex color
		final_vertexColor = vec3((0.5f*(1.0f-normalized_deflscale)+(vertexColor.x*normalized_deflscale)),
								(0.0f*(1.0f-normalized_deflscale)+(vertexColor.y*normalized_deflscale)),
								(1.0f*(1.0f-normalized_deflscale)+(vertexColor.z*normalized_deflscale)));
	}
	else if(pt_type == 1.0f)
	{
		// pt_type 1 is the vector end point
		// Scale based on model
		float node_circe_radii = 0.005f;
		float defl_ratio = deflscale * (node_circe_radii/ geom_scale);

		// Find the vector value from the vector origin
		vec2 vector_value = pt_position - vector_origin;

		// Scale the vector end point
		vec2 vector_end_position = vec2(vector_origin.x + (vector_value.x * defl_ratio), vector_origin.y - (vector_value.y * defl_ratio));

		// apply Transformation to the vector end position
		finalPosition = scaledModelMatrix * vec4(vector_end_position,0.0f,1.0f) * panTranslation;

		// Update the color based on cycle time
		final_vertexColor = vec3((0.5f*(1.0f-normalized_deflscale)+(vertexColor.x*normalized_deflscale)),
								 (0.0f*(1.0f-normalized_deflscale)+(vertexColor.y*normalized_deflscale)),
								 (1.0f*(1.0f-normalized_deflscale)+(vertexColor.z*normalized_deflscale)));
	}
	else if(pt_type == 2.0f)
	{
		// pt_type 2 is the arrow head pt 1
		float node_circe_radii = 0.005f;
		float defl_ratio = deflscale * (node_circe_radii/ geom_scale);
		float arrow_size = 0.2f * defl_ratio;
		
		// Find the vector value from the vector origin
		vec2 vector_value = pt_position - vector_origin;

		// Scale the vector end point
		vec2 vector_end_position = vec2(vector_origin.x + (vector_value.x * defl_ratio), vector_origin.y - (vector_value.y * defl_ratio));

		// Find the vector arrow direction
		vec2 arrowDirection = normalize(vector_end_position - vector_origin);

		// Arrow point along the line without rotation
		vec2 leftPoint = vector_end_position + (arrow_size * normalize(vec2(arrowDirection.y, -arrowDirection.x)));

		// Values for rotation
		float angle = radians(105.0); // 30 degrees in radians
		mat2 rotationMatrix = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));

		vec2 rotatedLeftPoint = vector_end_position + (rotationMatrix * (leftPoint - vector_end_position));

		// apply Transformation to the vector end position
		finalPosition = scaledModelMatrix * vec4(rotatedLeftPoint,0.0f,1.0f) * panTranslation;

		// Update the color based on cycle time
		final_vertexColor = vec3((0.5f*(1.0f-normalized_deflscale)+(vertexColor.x*normalized_deflscale)),
								 (0.0f*(1.0f-normalized_deflscale)+(vertexColor.y*normalized_deflscale)),
								 (1.0f*(1.0f-normalized_deflscale)+(vertexColor.z*normalized_deflscale)));

	}
	else if(pt_type == 3.0f)
	{
		// pt_type 3 is the arrow head pt 2
		float node_circe_radii = 0.005f;
		float defl_ratio = deflscale * (node_circe_radii/ geom_scale);
		float arrow_size = 0.2f * defl_ratio;
		
		// Find the vector value from the vector origin
		vec2 vector_value = pt_position - vector_origin;

		// Scale the vector end point
		vec2 vector_end_position = vec2(vector_origin.x + (vector_value.x * defl_ratio), vector_origin.y - (vector_value.y * defl_ratio));

		// Find the vector arrow direction
		vec2 arrowDirection = normalize(vector_end_position - vector_origin);

		// Arrow point along the line without rotation
		vec2 rightPoint  = vector_end_position + (arrow_size * normalize(vec2(arrowDirection.y, -arrowDirection.x)));

		// Values for rotation
		float angle = radians(75.0); // 30 degrees in radians
		mat2 rotationMatrix = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));

		vec2 rotatedRightPoint = vector_end_position + (rotationMatrix * (rightPoint - vector_end_position));

		// apply Transformation to the vector end position
		finalPosition = scaledModelMatrix * vec4(rotatedRightPoint,0.0f,1.0f) * panTranslation;

		// Update the color based on cycle time
		final_vertexColor = vec3((0.5f*(1.0f-normalized_deflscale)+(vertexColor.x*normalized_deflscale)),
								 (0.0f*(1.0f-normalized_deflscale)+(vertexColor.y*normalized_deflscale)),
								 (1.0f*(1.0f-normalized_deflscale)+(vertexColor.z*normalized_deflscale)));

	
	}

	v_Color = vec4(final_vertexColor,transparency);

	// Final position passed to fragment shader
	gl_Position = finalPosition;
}