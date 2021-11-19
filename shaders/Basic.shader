#shader vertex
#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_MVP;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_MVP * vec4(a_Pos, 1.0);
    v_FragPos = vec3(u_Model * vec4(a_Pos, 1.0));
    v_Normal = vec3(u_Model * vec4(normalize(a_Normal), 1.f));
}

#shader fragment
#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec4 u_Color;
// uniform sampler2D u_Texture;

void main()
{
	vec3 lightPos = vec3(10.f, 10.f, 10.f);
	vec3 lightColor = vec3(1.f);
	vec3 objectColor = vec3(0.3f, 0.2f, 0.5f);

	float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(v_Normal);

	vec3 lightDir = normalize(lightPos - v_FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.f);
}