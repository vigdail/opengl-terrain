#version 450 core

in vec3 fragPos;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

layout(std140, binding = 0) uniform Camera {
    vec3 camera_position;
    mat4 view;
    mat4 projection;
};

uniform DirectionalLight sun;
uniform float planet_radius;
uniform float atmosphere_radius;
uniform float hR;
uniform float hM;
uniform float g;
uniform vec3 beta_R;
uniform float beta_M;

uniform int view_dir_samples = 16;
uniform int sun_dir_samples = 8;

out vec4 fragColor;

const float PI = 3.141592;

vec2 rsi(vec3 r0, vec3 rd, float sr) {
    // ray-sphere intersection that assumes
    // the sphere is centered at the origin.
    // No intersection when result.x > result.y
    float a = dot(rd, rd);
    float b = 2.0 * dot(rd, r0);
    float c = dot(r0, r0) - (sr * sr);
    float d = (b*b) - 4.0*a*c;
    return (d < 0.0) 
        ? vec2(1e5,-1e5) 
        : vec2(
            (-b - sqrt(d))/(2.0*a),
            (-b + sqrt(d))/(2.0*a)
        );
}

void main() { 
    if (fragPos.y < 0) {
        discard;
    }

    vec3 r = normalize(fragPos);
    vec3 r0 = camera_position + vec3(0, planet_radius, 0);

    // Calculate the step size of the primary ray.
    vec2 p = rsi(r0, r, atmosphere_radius);
    if (p.x > p.y) {
        fragColor.rgb = vec3(0,0,0);
        return;
    }
    p.y = min(p.y, rsi(r0, r, planet_radius).x);
    float view_step_size = (p.y - p.x) / float(view_dir_samples);

    // Initialize accumulators for Rayleigh and Mie scattering.
    vec3 total_R = vec3(0,0,0);
    vec3 total_M = vec3(0,0,0);

    // Initialize optical depth accumulators for the primary ray.
    float i_od_R = 0.0;
    float i_od_M = 0.0;

    // Calculate the Rayleigh and Mie phases.
    float mu = dot(r, sun.direction);
    float mumu = mu * mu;
    float gg = g * g;
    float phase_R = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float phase_M = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

    for (int i = 0; i < view_dir_samples; i++) {
        vec3 i_pos = r0 + r * (i + 0.5) * view_step_size;
        float i_height = length(i_pos) - planet_radius;

        float od_step_R = exp(-i_height / hR) * view_step_size;
        float od_step_M = exp(-i_height / hM) * view_step_size;

        i_od_R += od_step_R;
        i_od_M += od_step_M;

        // Calculate the step size of the secondary ray.
        float sun_step_size = rsi(i_pos, sun.direction, atmosphere_radius).y / float(sun_dir_samples);

        float j_od_R = 0.0;
        float j_od_M = 0.0;

        // Sample the secondary ray.
        for (int j = 0; j < sun_dir_samples; j++) {
            vec3 j_pos = i_pos + sun.direction * (j + 0.5) * sun_step_size;
            float j_height = length(j_pos) - planet_radius;

            j_od_R += exp(-j_height / hR) * sun_step_size;
            j_od_M += exp(-j_height / hM) * sun_step_size;
        }

        vec3 tau = beta_M * (i_od_M + j_od_M) + beta_R * (i_od_R + j_od_R);
        vec3 attenuation = exp(-tau);

        // Accumulate scattering.
        total_R += od_step_R * attenuation;
        total_M += od_step_M * attenuation;
    }

    fragColor.rgb = sun.intensity * (phase_R * beta_R * total_R + phase_M * beta_M * total_M);

    // @TODO: Move this to post-processing shader
    fragColor.rgb = vec3(1.0) - exp(-fragColor.rgb * 0.5);
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/2.2));
}
