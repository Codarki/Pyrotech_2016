
float3 from_srgb_to_linear(float3 color)
{
	// convert from gamma 2.2 to gamma 1.0
	return pow(abs(color), 2.2);
    //return pow((abs(color) + 0.055)/ 1.055, 2.4);
}

float3 from_linear_to_srgb(float3 color)
{
	// convert from linear gamma to gamma 2.2
	return pow(abs(color), 1.0 / 2.2);
	//return pow(1.055 * abs(color), 1.0 / 2.4) - 0.055;
}

float3 lambertian_diffuse_term(float3 N, float3 L, float3 diffuse_coeff)
{
    return diffuse_coeff;
    //return max(0, dot(N, L)) * diffuse_coeff;
}

float2 to_screen_space(float4 position_in_clip)
{
    /*float2 result = position_in_clip.xy / position_in_clip.w;
    result.y = -result.y;
    result *= 0.5f;
    result += 0.5f;
    */
    float2 scrPos = position_in_clip.xy / position_in_clip.w;
    float2 result = float2(
        (1 + scrPos.x) / 2 + (0.5 / 1280),
        (1 - scrPos.y) / 2 + (0.5 / 720)
    );
    
    return result;
}

// Roughness coefficient, between 0.0 and 1.0
// When roughness is 0.0 (a completely smooth surface) the model exhibits Lambertian behaviour.
float3 oren_nayar_diffuse_term(
	float3 N,
	float3 L,
	float3 V,
	float roughness_coeff)
{
    float N_dot_V = dot(N, V);
    float N_dot_L = dot(N, L);
 
    float gamma = dot(V - N * N_dot_V, L - N * N_dot_L);
    float roughness_squared = roughness_coeff * roughness_coeff;
    float A = 1.0f - 0.5f * (roughness_squared / (roughness_squared + 0.57f));
    float B = 0.45f * (roughness_squared / (roughness_squared + 0.09));
	
	// texture loopkup optimized
    //
    //	// The two dot-products will be in the range of 
    //	// 0.0 to 1.0 which is perfect for a texture lookup:
	//	float tc = float2((VdotN + 1.0f) / 2.0f, (LdotN + 1.0f) / 2.0f);
	//	float C = texSinTanLookup.Sample( DefaultSampler, tc ).r;
    //
	
	// heavier version
	float alpha = max(acos(N_dot_V), acos(N_dot_L));
	float beta  = min(acos(N_dot_V), acos(N_dot_L));
	float C = sin(alpha) * tan(beta);
 
    float final = (A + B * max(0.0f, gamma) * C);
	//return saturate(N_dot_L) * final;
	return saturate(final);
	//return max(0.0f, N_dot_L) * final;
}

// van Ouwerkerk's rewrite
// http://shaderjvo.blogspot.fi/2011/08/van-ouwerkerks-rewrite-of-oren-nayar.html
float oren_nayar_diffuse_term_ouwerkerk(
	float3 N,
	float3 L,
	float3 V,
	float roughness_coeff)
{
	//N = normalize(N + V * 10);
	
    float N_dot_V = dot(N, V);
    float N_dot_L = dot(N, L);
    float roughness_squared = roughness_coeff * roughness_coeff;
	
	// The 0.57 in A is actually a 0.33 in the original formula, but a footnote says that discrepancies caused by the lack of the interreflection component can be partially compensated by using 0.57 instead.
	float2 oren_nayar_fraction = roughness_squared / (roughness_squared + float2(0.57, 0.09));
	//float2 oren_nayar_fraction = roughness_squared / (roughness_squared + float2(0.33, 0.09));
	float2 oren_nayar = float2(1, 0) + float2(-0.5, 0.45) * oren_nayar_fraction;
	
	// Theta and phi
	float2 cos_theta = saturate(float2(N_dot_L, N_dot_V));
	float2 cos_theta2 = cos_theta * cos_theta;
	float sin_theta = sqrt((1 - cos_theta2.x) * (1 - cos_theta2.y));
	float3 light_plane = normalize(L - cos_theta.x * N);
	float3 view_plane = normalize(V - cos_theta.y * N);
	float cos_phi = saturate(dot(light_plane, view_plane));
	
	// Composition
	float diffuse_oren_nayar = cos_phi * sin_theta / max(cos_theta.x, cos_theta.y);
	float diffuse = cos_theta.x * (oren_nayar.x + oren_nayar.y * diffuse_oren_nayar);
	return diffuse;
}

float3 blinn_phong_specular_term(float3 N, float3 L, float3 V,
    float3 specular_coeff, float specular_exponent)
{
    float3 H = normalize(L + V);
    float N_dot_H = max(0, dot(N, H));
    
    return pow(N_dot_H, specular_exponent) * specular_coeff;
}

//
// Low roughness values, implying a smooth surface, generate the shiniest
// results with specular highlights similar to that seen with the Phong model
// 0.3 - 0.9
//
float cook_torrance_specular_term(float3 N, float3 L, float3 V,
    float roughness_value, float fresnel)
{
    float3 H = normalize(L + V);
    float N_dot_V = saturate(dot(N, V));
    float N_dot_L = saturate(dot(N, L));
    float N_dot_H = max(0.0001, dot(N, H));
    float V_dot_H = max(0.0001, dot(V, H));

    // Evaluate the geometric term
    float geo_numerator = 2.0f * N_dot_H;
    float geo_denominator = V_dot_H;
 
    float geo_b = (geo_numerator * N_dot_V) / geo_denominator;
    float geo_c = (geo_numerator * N_dot_L) / geo_denominator;
    float geo = min(1.0f, min(geo_b, geo_c));
    
    // Now evaluate the roughness term using beckmann’s distribution
    float roughness_squared = roughness_value * roughness_value;
    float roughness_a = 1.0f / (4.0f * roughness_squared * pow(N_dot_H, 4));
    //float roughness_a = 1.0f / (roughness_squared * pow(N_dot_H, 4));
    float roughness_b = N_dot_H * N_dot_H - 1.0f;
    float roughness_c = roughness_squared * N_dot_H * N_dot_H;
 
    float roughness_term = roughness_a * exp( roughness_b / roughness_c );
        
    // compute the specular term in the equation
    float Rs_numerator = fresnel * geo * roughness_term;
    //float Rs_denominator = max(0.0001, N_dot_V * N_dot_L);
    float Rs_denominator = N_dot_V * N_dot_L;
    float Rs = Rs_numerator / Rs_denominator;
    
    return max(0, Rs);
}
// http://en.wikipedia.org/wiki/List_of_refractive_indices

float fresnel_approximation_schlick(float3 L, float3 V,
    float ref_at_norm_incidence)
{
    float3 H = normalize(L + V);
    float V_dot_H = saturate(dot(V, H));

    float fresnel = pow(1.0f - V_dot_H, 5.0f);
    fresnel *= (1.0f - ref_at_norm_incidence);
    fresnel += ref_at_norm_incidence;
    return fresnel;
}

float fresnel_approximation_jari(float3 N, float3 V,
    float ref_at_norm_incidence)
{

    float N_dot_V = saturate(dot(N, V));

    float fresnel = pow(1.0f - N_dot_V, 5.0f);
    fresnel *= (1.0f - ref_at_norm_incidence);
    fresnel += ref_at_norm_incidence;
    return fresnel;
}

// Bidirectional Reflectance Distribution Function
float3 calculate_pixel_BRDF_blinn(float3 N, float3 L, float3 V,
    float3 diffuse_coeff, float3 specular_coeff, float specular_exponent)
{
    float fresnel = fresnel_approximation_schlick(L, V, 0.1);

    float3 diffuse_term = lambertian_diffuse_term(N, L, diffuse_coeff);
    float3 specular_term = blinn_phong_specular_term(N, L, V, specular_coeff,
        specular_exponent) * fresnel;
		    
    return diffuse_term + specular_term;
}

float3 calculate_pixel_BRDF_cook_torrance(
    float3 N, float3 L, float3 V,
    float3 diffuse_coeff, float3 specular_coeff,
	float roughness,
	float diffuse_roughness,
    float fresnel_normal_incidence, float3 incoming_reflection)
{
    float fresnel = fresnel_approximation_schlick(L, V, fresnel_normal_incidence);
    //float3 diffuse_term = lambertian_diffuse_term(N, L, diffuse_coeff);
	
    float3 diffuse_term = oren_nayar_diffuse_term_ouwerkerk(N, L, V, diffuse_roughness)
	    * diffuse_coeff;
    
    float3 specular_term = cook_torrance_specular_term(N, L, V,
        roughness, fresnel) * specular_coeff;
	
	diffuse_term *= (1 - fresnel);
			
    return diffuse_term + specular_term + incoming_reflection * fresnel;
}

float3 calculate_pixel_BRDF_cook_torrance_water(
    float3 N, float3 L, float3 V,
    float3 diffuse_coeff, float3 specular_coeff,
	float roughness,
	float diffuse_roughness,
    float fresnel_normal_incidence, float3 incoming_reflection)
{
    float fresnel = fresnel_approximation_jari(N, V, fresnel_normal_incidence);
	
    float3 diffuse_term = oren_nayar_diffuse_term_ouwerkerk(N, L, V, diffuse_roughness)
	    * diffuse_coeff;
    
    float3 specular_term = cook_torrance_specular_term(N, L, V,
        roughness, fresnel) * specular_coeff;
            
	diffuse_term *= (1 - fresnel);
			
    return diffuse_term + specular_term + incoming_reflection * fresnel;
}


float3 calculate_pixel_BRDF_cook_torrance_rock(float3 N, float3 L, float3 V,
    float3 diffuse_coeff,
	float3 specular_coeff,
	float roughness,
	float diffuse_roughness,
    float fresnel_normal_incidence, float3 incoming_reflection)
{
    float fresnel = 0.0;
    //float3 diffuse_term = lambertian_diffuse_term(N, L, diffuse_coeff);
	
    float3 diffuse_term = oren_nayar_diffuse_term_ouwerkerk(N, L, V, diffuse_roughness)
	    * diffuse_coeff;
    
    float3 specular_term = cook_torrance_specular_term(N, L, V,
        roughness, fresnel) * specular_coeff;
            
	diffuse_term *= (1 - fresnel);
			
    return diffuse_term + specular_term + incoming_reflection;
}

float3 calculate_pixel_color_blinn(float3 N, float3 L, float3 V,
    float3 diffuse_coeff, float3 specular_coeff, float specular_exponent,
    float3 light)
{    
    float3 BRDF = calculate_pixel_BRDF_blinn(N, L, V, diffuse_coeff,
        specular_coeff, specular_exponent);
    
    float N_dot_L = saturate(dot(N, L));
        
    return BRDF * light * N_dot_L;
}
