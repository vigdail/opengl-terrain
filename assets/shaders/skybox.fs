// Atmosphere scattering code from here:
// https://github.com/shff/opengl_sky
#version 450 core

in vec3 fragPos;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

uniform DirectionalLight light;
uniform float Br = 0.0025;
uniform float Bm = 0.0003;
uniform float g =  0.9800;
uniform vec3 color = vec3(0.650, 0.570, 0.475);

out vec4 fragColor;

vec3 Kr = Br / pow(color, vec3(4.0));
vec3 Km = Bm / pow(color, vec3(0.84));

void main() {
    if (fragPos.y < 0) {
        discard;
    }

    float mu = dot(normalize(fragPos), normalize(light.direction));
    // @TODO: Replace this with integration
    vec3 extinction = mix(
        exp(-exp(-((fragPos.y + light.direction.y * 4.0)
                    * (exp(-fragPos.y * 16.0) + 0.1) / 80.0) / Br) 
                    * (exp(-fragPos.y * 16.0) + 0.1) * Kr / Br) 
                    * exp(-fragPos.y * exp(-fragPos.y * 8.0 ) * 4.0) 
                    * exp(-fragPos.y * 2.0) * 4.0, vec3(1.0 - exp(light.direction.y)) 
                    * 0.2, -light.direction.y * 0.2 + 0.5);
    fragColor.rgb = 3.0 / (8.0 * 3.14) * (1.0 + mu * mu) * (Kr + Km * (1.0 - g * g) / (2.0 + g * g) / pow(1.0 + g * g - 2.0 * g * mu, 1.5)) / (Br + Bm) * extinction;

    // @TODO: Move this to post-processing shader
    fragColor.rgb = vec3(1.0) - exp(-fragColor.rgb * 0.5);
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/2.2));
}
