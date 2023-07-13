#define M_PI 3.141592653589793f

vec3 PointLight(vec3 pointLightPosition, vec3 pointLightColor, vec3 fragPos, float beta, float g) {
    vec3 dir = pointLightPosition - fragPos;
	vec3 lightDir = normalize(dir);
	float decay = pow(g / length(dir), beta);
    return pointLightColor * decay;
}

vec3 DirectionalLight(vec3 lightDir, vec3 lightColor) {
    return lightColor;
}

vec3 Spotlight(vec3 lightPos, vec3 lightCol, vec3 lightDir, vec3 fragPos, float decayExp, float baseDist, float cosin, float cosout) {
	vec3 dir = lightPos - fragPos;
	vec3 lightDirection = normalize(-dir);
	float decay = pow(baseDist / length(dir), decayExp);
	vec3 lightColor = lightCol;
	float cosalpha = dot(lightDirection, normalize(-lightDir));
	float coneDimming = clamp((cosalpha - cosout) / (cosin - cosout), 0, 1);
	lightColor = lightColor * decay * coneDimming;

    return lightColor;
}

float Gggx(vec3 n, vec3 a, float rho) {
	float pow2 = pow(clamp(dot(n, a), 0.0001f, 1), 2);
	float term2 = (1 - pow2) / pow2;
	float rho2 = rho * rho;
	return 2 / (1 + sqrt(1 + rho2 * term2));
}

vec3 LambertDiffuse(vec3 Md, vec3 L, vec3 N) {
    //vec3 N  - normal vector to the surface
	//vec3 L  - light vector (from the light model)
	//vec3 Md - main color of the surface
    return Md * clamp(dot(L, N), 0, 1);
}

vec3 PhongSpecular(vec3 V, vec3 N, vec3 L, vec3 Ms, float gamma) {
	//vec3 V  - direction of the viewer
	//vec3 N  - normal vector to the surface
	//vec3 L  - light vector (from the light model)
	//vec3 Ms - specular color of the surface
	//float gamma - Exponent for power specular term
	vec3 R = 2 * N * dot(L, N) - L;
	vec3 phong = Ms * pow(clamp(dot(V, R), 0, 1), gamma);
	return phong;
} //Combine with lambert as <lambert> + <blinn>

vec3 BlinnSpecular(vec3 V, vec3 N, vec3 L, vec3 Ms, float gamma) {
	//vec3 V  - direction of the viewer
	//vec3 N  - normal vector to the surface
	//vec3 L  - light vector (from the light model)
	//vec3 Ms - specular color of the surface
	//float gamma - Exponent for power specular term
	vec3 R = normalize(L + V);
	vec3 blinn = Ms * pow(clamp(dot(N, R), 0, 1), gamma);
	return blinn;
} //Combine with lambert as <lambert> + <blinn>

vec3 OrenNayarDiffuse(vec3 V, vec3 N, vec3 L, vec3 Md, float sigma) {
	//vec3 V  - direction of the viewer
	//vec3 N  - normal vector to the surface
	//vec3 L  - light vector (from the light model)
	//vec3 Md - main color of the surface
	//float sigma - Roughness of the model
	float theta_i = acos(dot(L, N));
	float theta_r = acos(dot(V, N));
	float alpha = max(theta_i, theta_r);
	float beta = min(theta_i, theta_r);
	float A = 1 - 0.5f * (sigma * sigma) / (sigma * sigma + 0.33f);
	float B = 0.45f * sigma * sigma / (sigma * sigma + 0.09f);
	vec3 vi = normalize(L - (dot(L, N) * N));
	vec3 vr = normalize(V - (dot(V, N) * N));
	float G = max(0, dot(vi, vr));
	vec3 L1 = Md * clamp(dot(L, N), 0, 1);
	vec3 diffuse = L1 * (A + B * G * sin(alpha) * tan(beta));
	return diffuse;
} //No specular highlights

vec3 GGXDiffuseSpecular(vec3 V, vec3 N, vec3 L, vec3 Md, float F0, float metallic, float roughness) {
    //vec3 V  - direction of the viewer
	//vec3 N  - normal vector to the surface
	//vec3 L  - light vector (from the light model)
	//vec3 Md - main color of the surface
	//float F0 - Base color for the Fresnel term
	//float metallic - parameter that mixes the diffuse with the specular term.
	//                 in particular, parmeter K seen in the slides is: float K = 1.0f - metallic;
	//float roughness - Material roughness (parmeter rho in the slides).
	//specular color Ms is not passed, and implicitely considered white: vec3 Ms = vec3(1.0f);
	float K = 1.0f - metallic;
	vec3 Ms = vec3(1, 1, 1);
	vec3 H = normalize(L + V);
	float D = (roughness * roughness) / (M_PI * max(pow(pow(clamp(dot(N, H), 0, 1), 2) * (roughness * roughness - 1) + 1, 2), 0.001f));
	float F = F0 + (1 - F0) * pow(1 - clamp(dot(V, H), 0, 1), 5);
	float G = Gggx(N, V, roughness) * Gggx(N, L, roughness);
	vec3 specular = Ms * (D * F * G) / (4 * clamp(dot(V, N), 0.001f, 1));
	vec3 diffuse = LambertDiffuse(Md, L, N);
	return K * diffuse + (1 - K) * specular;
} //Diffuse + Specular already combined!

