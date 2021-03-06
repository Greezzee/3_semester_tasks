
//Локальные переменные
uniform sampler2D texture;
uniform vec4 global_color;

//Глобальные переменный
uniform int source_count;
uniform vec2 pos[128];
uniform vec4 light_color[128];
uniform float full_light_dist[128];
uniform float any_light_dist[128];
uniform float softness[128];

uniform float pixel_size;

uniform vec4 global_light_color;

void main() {

	
	gl_FragColor = vec4(0, 0, 0, 0);
	vec4 text_color = texture2D(texture, gl_TexCoord[0].xy);
	gl_FragColor += text_color * global_light_color;
	for (int i = 0; i < source_count; i++) {
		vec2 d = gl_FragCoord.xy - pos[i];
		d -= mod(d, pixel_size);
		float dist = length(d);
		if (dist < full_light_dist[i])
			gl_FragColor += text_color * light_color[i];
		else {
			dist -= full_light_dist[i];
			gl_FragColor += text_color * light_color[i] * pow(max(0.0, 1.0 - dist / any_light_dist[i]), softness[i]);
		}
	}

	gl_FragColor = min(gl_FragColor, text_color);
	gl_FragColor.w = text_color.w;
	gl_FragColor *= global_color;
}