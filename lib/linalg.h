typedef struct { float x, y, z; } Vec3;

typedef struct { float x, y, z, w; } Vec4;

typedef struct { float m[4][4]; } Mat4;

typedef struct { Vec3 position, target, up; } Camera;

/**
 * @brief	multiplies a 4x4 matrix with a 4-dimensional vector.
 *
 * @param	mat	A 4x4 matrix
 * @param	vec	A 4-d vector
 *
 * @return	mat * vec, a 4d matrix of type `Vec4`
 */
Vec4 multiplyMat4Vec4(Mat4 mat, Vec4 vec);

Vec3 subtractVec3(Vec3 a, Vec3 b);
Vec3 normalizeVec3(Vec3 v);
Vec3 crossVec3(Vec3 a, Vec3 b);
float dotVec3(Vec3 a, Vec3 b);
Vec3 negativeVec3(Vec3 v);

/**
 * @brief	Creates a LookAt (View) Matrix. Conceptually, this is the inverse of the camera's own transformation.
 *
 * @param	camera	The camera which does the viewing.
 */
Mat4 getViewMatrix(Camera camera);

/**
 * @brief	Creates a perspective projection matrix
 *
 * @param	fov		The angle in radians from the top to the bottom of the viewing frustum
 * @param	aspect	The ratio of the width to height of the frustum
 * @param	near	The distance of the near clipping plane.
 * @param	far		The distance of the far clipping plane.
 */
Mat4 getProjectionMatrix(float fov, float aspect, float near, float far);

/**
 * @brief	Creates a model matrix to transform a model into world space. Default is the identity matrix.
 *
 * @return	The model matrix created
 */
Mat4 getModelMatrix();

float deg2rad(float deg);
