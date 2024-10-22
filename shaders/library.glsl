#define M_PI 3.141592653589793f

vec3 PointLight(vec3 pointLightPosition, vec3 pointLightColor, vec3 fragPos, float beta, float g) {
    vec3 dir = pointLightPosition - fragPos;
	float decay = pow(g / length(dir), beta);
    return pointLightColor * decay;
}

vec3 DirectionalLight(vec3 lightDir, vec3 lightColor) {
    return lightColor;
}

vec3 Spotlight(vec3 lightPos, vec3 lightCol, vec3 spotDir, vec3 fragPos, float decayExp, float baseDist, float cosin, float cosout) {
	vec3 dir = lightPos - fragPos;
	vec3 lightDirection = normalize(dir);
	float decay = pow(baseDist / length(dir), decayExp);
	vec3 lightColor = lightCol;
	float cosalpha = dot(lightDirection, -normalize(spotDir));
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
	vec3 R = -reflect(L, N);
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

// Spherical Harmonics
vec3 sh(const in vec3 normal) {
	const vec3 c[9] = {
			vec3(0.136158, 0.163803, 0.259454),
			vec3(0.0108949, -0.0153887, -0.0450592),
			vec3(0.00551577, -0.0134505, -0.00767747),
			vec3(0.0531763, 0.0883831, 0.0873933),
			vec3(-0.00628636, 0.0018491, -0.00416013),
			vec3(0.00671475, -0.0019917, -0.0099933),
			vec3(-0.0417106, -0.0271285, -0.0179189),
			vec3(-0.00668723, 0.0055851, 0.00755085),
			vec3(0.0642979, 0.0727677, 0.058611)
	};
	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	vec3 result = (
		c[0] +

		c[1] * x +
		c[2] * y +
		c[3] * z +

		c[4] * z * x +
		c[5] * y * z +
		c[6] * y * x +
		c[7] * (3.0 * z * z - 1.0) +
		c[8] * (x*x - y*y)
	);
	return max(result, vec3(0.0));
}

