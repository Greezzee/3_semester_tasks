#version 110

uniform sampler2D texture;
uniform vec2 pos[6];
uniform vec4 color[6];

const int source_count = 40;
const float EPS = 0.0001;
uniform float time;


float det (float a, float b, float c, float d) {
    return a * d - b * c;
}

bool betw(float a, float b, float x)
{
	return (a <= x + EPS && x <= b + EPS) || (b <= x + EPS && x <= a + EPS);
}

float intersect (vec4 wall, vec4 ray) {
	
	vec3 w, r;

	w.x = wall.y - wall.w;
	w.y = wall.z - wall.x;
	w.z = -w.x * wall.x - w.y * wall.y;

	r.x = ray.y - ray.w;
	r.y = ray.z - ray.x;
	r.z = -r.x * ray.x - r.y * ray.y;
	
    float zn = det(r.x, r.y, w.x, w.y);
    if (abs (zn) < EPS)
        return 0.0;
	vec2 res;
    res.x = - det (r.z, r.y, w.z, w.y) / zn;
    res.y = - det (r.x, r.z, w.x, w.z) / zn;

	
	if (!(betw(wall.x, wall.z, res.x) && betw(wall.y, wall.w, res.y)))
		return 0.0;
	if (!(betw(ray.x, ray.z, res.x) && betw(ray.y, ray.w, res.y)))
		return 0.0;
    return 1.0;
}

void main()
{
	vec4 wall[3]; 
	wall[0] = vec4(300.0, 360.0, 640.0, 460.0);
	wall[1] = vec4(640.0, 460.0, 480.0, 120.0);
	wall[2] = vec4(480.0, 120.0, 300.0, 360.0);
	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	for(int i = 0; i < 1; i++) {
		float dist = length(gl_FragCoord.xy - pos[i]);
		vec4 light = (texture2D(texture, gl_TexCoord[0].xy) - (vec4(1.0, 1.0, 1.0, 1.0) - color[i])) * pow(2.0 / dist, 0.2);
		for (int j = 0; j < 3; j++) {
			float k = intersect(wall[j], vec4(gl_FragCoord.xy, pos[i]));
			if (k >= EPS) {
				light *= 0.5;
			}
		}
		gl_FragColor = min(gl_FragColor + light, texture2D(texture, gl_TexCoord[0].xy));
	}
	/*
	for (int i = 0; i < 2; i++) {
		vec2 d = gl_FragCoord.xy - pos[i];
		vec2 d1;
		d1.x = d.x * cos(45.0) + d.y * sin(45.0);
		d1.y = d.y * cos(45.0) - d.x * sin(45.0);
		if (pow(abs(d1.x), 0.7) + pow(d1.y, 2.0) <= 40.0)
			gl_FragColor += vec4(0.7, 0.7, 0.7, 0.0);
		if (pow(abs(d1.y), 0.7) + pow(d1.x, 2.0) <= 40.0)
			gl_FragColor += vec4(0.7, 0.7, 0.7, 0.0);
	}
	*/

	//gl_FragColor = texture2D(texture, gl_TexCoord[0].xy + vec2( 2.0 * (gl_TexCoord[0].x - 0.5) * abs(0.5 - gl_TexCoord[0].y), 0.0));// + 0.02 * vec2(0.0, cos(90.0 * gl_TexCoord[0].x + time));
}