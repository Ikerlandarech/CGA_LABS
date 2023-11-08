varying vec3 v_position;

uniform mat4 u_viewprojection;
uniform vec3 u_camera_position;
uniform mat4 u_model;
uniform mat4 u_inverse_model;
uniform float u_time;
uniform vec4 u_color;

uniform float u_step;
uniform float u_brightness;

uniform sampler3D u_texture; //texture needed to compute the texture coordinates

void main(){
    vec4 final_color = vec4(0.0); //initialize the final color
    vec3 r_dir = normalize(v_position - (u_inverse_model * vec4(u_camera_position, 1.0)).xyz); //ray dir
    vec3 step_vector = r_dir * u_step; //step vector
    vec3 sample_pos = v_position;
	float d;
	vec4 sample_color;

    for (int i=0; i<1000; ++i){
        d = texture(u_texture, (sample_pos + 1.0)/2.0).x; //ray_direction ->loop:volume sampling -- d=density
        sample_color = vec4(d,d,d,d); //color of the composition ->loop:classification
        sample_color.rgb *= sample_color.a; //Multiplying the colors by the opacity
        final_color += u_step * (1.0 - final_color.a) * sample_color; // final color acumulated -> composition
        sample_pos = sample_pos + r_dir * u_step;
        
        if (final_color.a >= 1.0){
            break;
        }
        if((sample_pos.x > 1.0) || (sample_pos.y > 1.0)||(sample_pos.z > 1.0) || (sample_pos.x < -1.0)||(sample_pos.y < -1.0)||(sample_pos.z < -1.0)) {
            break;
        }
    }
    gl_FragColor = final_color * u_brightness;
}