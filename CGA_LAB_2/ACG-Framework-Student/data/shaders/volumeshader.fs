varying vec3 v_position;

uniform mat4 u_viewprojection;
uniform vec3 u_camera_position;
uniform mat4 u_model;
uniform mat4 u_inverse_model;
uniform float u_time;
uniform vec4 u_color;
uniform float alpha_discard;

uniform float u_step;
uniform float u_brightness;
uniform sampler2D noise_texture; //noise texture
uniform bool jittering_1; //blue noise texture approach
uniform bool jittering_2; //pseudorandom-looking function approach
uniform sampler2D TF; // transfer function texture
uniform bool transfer;
uniform sampler3D u_texture; //texture needed to compute the texture coordinates
uniform bool clipping;
uniform vec4 plane_clipping;
uniform bool isosurface;

uniform float gradient_threshold;
uniform float h;

uniform vec3 light_position;
uniform vec3 ambient_light;
uniform vec3 diffuse_light;
uniform vec3 specular_light;
uniform vec3 ka;
uniform vec3 kd;
uniform float alpha;

float rand(vec2 co){ //jittering pseudorandom-looking function.
	return fract(sin(dot(co,vec2(12.9898, 78.233)))*43758.5453);
}

//Computing the Phong lighting model for a given normal vector and texture position:
vec3 phong(vec3 v_normal, vec3 s_position_text) {
    //Reading the pixel RGBA from the texture at position v_uv:
    vec4 color = u_color;

    //Normalizing the surface normal vector:
    vec3 N = normalize(v_normal);

    //Computing the normalized direction vector from the light source to the surface:
    vec3 L = normalize(light_position - s_position_text);
    
    //Computing the dot product of the normalized light direction and the surface normal:
    float LN = dot(L, N);
    LN = clamp(LN, 0.0, 1.0);

    //Computing the normalized view direction vector from the surface to the camera:
    vec3 V = normalize(u_camera_position - s_position_text);

    //Computing the reflected light direction:
    vec3 R = normalize(reflect(-L, N));

    //Computing the dot product of the reflected light direction and the view direction:
    float RV = dot(R, V);
    RV = clamp(RV, 0.0, 1.0);

    //Computing the specular reflection coefficient based on the texture color alpha channel:
    vec3 ks = color.xyz * color.w;

    //Computing the ambient, diffuse, and specular components of the Phong model:
    vec3 ambient = ka * ambient_light * color.xyz;
    vec3 diffuse = kd * (LN) * diffuse_light * color.xyz;
    vec3 specular = ks * pow(RV, alpha) * specular_light;

    //Computing the final Phong lighting intensity:
    vec3 I = ambient + diffuse + specular;

    return I;
}

vec3 gradient(vec3 s_position_tex){ //gradient perpendicular to the isosurfaces.

	//First we define the positions in the x, y, and z directions with a small offset 'h'.
	vec3 xh1 = vec3(s_position_tex.x + h, s_position_tex.y, s_position_tex.z);
	vec3 xh2 = vec3(s_position_tex.x - h, s_position_tex.y, s_position_tex.z);
	vec3 yh1 = vec3(s_position_tex.x, s_position_tex.y+h, s_position_tex.z);
	vec3 yh2 = vec3(s_position_tex.x, s_position_tex.y-h, s_position_tex.z);
	vec3 zh1 = vec3(s_position_tex.x, s_position_tex.y, s_position_tex.z + h);
	vec3 zh2 = vec3(s_position_tex.x, s_position_tex.y, s_position_tex.z - h);
	
	//Sample intensity values at the defined positions:
	float i_xh1 = texture3D(u_texture, xh1).x;
	float i_xh2 = texture3D(u_texture, xh2).x;
	float i_yh1 = texture3D(u_texture, yh1).y;
	float i_yh2 = texture3D(u_texture, yh2).y;
	float i_zh1 = texture3D(u_texture, zh1).z;
	float i_zh2 = texture3D(u_texture, zh2).z;

	//Computing the intensity differences in each direction:
	float i_xh = i_xh1 - i_xh2;
	float i_yh = i_yh1 - i_yh2;
	float i_zh = i_zh1 - i_zh2;
	
	//Computing the gradient vector and normalizing it:
	vec3 gradient = vec3(i_xh, i_yh, i_zh) / (2.0 * h);
	gradient = normalize(gradient);
	vec3 normal = 1.0 - gradient;

	return normal;
}

void main(){
    vec3 r_dir = normalize(v_position - (u_inverse_model * vec4(u_camera_position, 1.0)).xyz); //ray dir
    vec3 step_vector = r_dir * u_step; //step vector
    vec3 sample_pos = v_position;
    vec3 offset_1 = texture2D(noise_texture, gl_FragCoord.xy/128.0).xyz; //texture width
    vec2 co = gl_FragCoord.xy; //rand input
    float offset_2 = rand(co); //function defined above
	float d;
    vec3 normal = vec3(0.0); //normal perpendicular to the isosurfaces.
    vec4 sample_color;
    vec4 phong_color = vec4(0.0); //phong color initialization.
    vec4 final_color = vec4(0.0); //initialization of the final color

    //adding the offset to the first sample using both approaches:
    if (jittering_1){ //blue noise texture approach [activate from ImGui]
		sample_pos += offset_1 * step_vector;
	}
	if (jittering_2){ //pseudorandom-looking function approach [activate from ImGui]
		sample_pos += offset_2 * step_vector;
	}
    for (int i=0; i<1000; ++i){
        vec3 s_position_text = (sample_pos + vec3(1.0)) / 2.0;
        d = texture(u_texture, s_position_text).x; //ray_direction ->loop:volume sampling -- d=density
        sample_color = vec4(d,d,d,d); //color of the composition ->loop:classification
        sample_color.rgb *= sample_color.a; //Multiplying the colors by the opacity
        
        if (isosurface){ //Isosurface [activate from ImGui]
			if (d > gradient_threshold){
				normal = gradient(s_position_text);
				phong_color = vec4(phong(normal,s_position_text), 1.0);
                if (clipping){
					float clip = sample_pos.x * plane_clipping.x + sample_pos.y * plane_clipping.y + sample_pos.z * plane_clipping.z + plane_clipping.w;
					vec4 aux_color = final_color;
					if (clip > 0.0){ //Checking if the sample point is in the side we want to hide.
						final_color = aux_color;
					}
					else {
						final_color = phong_color;
					}
					break;
				}
			}
		}
        
        if (transfer){ //TF [activate from ImGui]
			vec4 colorTF = texture2D(TF,vec2(d,1));
			sample_color *= colorTF;
		}

        if (clipping){
			float clip = sample_pos.x * plane_clipping.x + sample_pos.y * plane_clipping.y + sample_pos.z * plane_clipping.z + plane_clipping.w;
			vec4 aux_color = final_color;
			if (clip > 0.0){ //Checking if the sample point is in the side we want to hide.
					final_color = aux_color;
			}
            else {
				final_color += u_step * (1.0 - final_color.a) * sample_color * u_color;
			}
		}

        else {
			vec4 aux_color = final_color;
			if (transfer) {
				final_color = aux_color;
			}
            else {
				final_color += u_step * (1.0 - final_color.a) * sample_color * u_color;
			}
		}

        sample_pos = sample_pos + step_vector;
         
        if ((sample_pos.x > 1.0) || (sample_pos.y > 1.0)||(sample_pos.z > 1.0) || (sample_pos.x < -1.0)||(sample_pos.y < -1.0)||(sample_pos.z < -1.0)) {
            break;
        }
        if (final_color.a >= 1.0){
            break;
        }
    }
    if (final_color.a <= alpha_discard){
		discard;
	}
    gl_FragColor = final_color * u_brightness;
}