#include "simple_math/QuaternionHelpers.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace simple_math
{
Quaternion QuaternionHelpers::quaterion_from_parameters(const double b0,
                                                        const double b1,
                                                        const double b2,
                                                        const double b3)
{
    return Quaternion(b0, b1, b2, b3);
}

Quaternion QuaternionHelpers::quaterion_from_axis_angle(const Vector3D axis, const double angle)
{
    Eigen::AngleAxis<FloatType> angle_axis(angle, axis);
    return Quaternion(angle_axis);
}

Quaternion QuaternionHelpers::quaterion_from_axis_angle(const int axis, const double angle)
{
    if (axis < 0 || axis > 2)
        throw _UTIL_MSG_EXCEPT("Axis index must be 0 to 2.");

    Vector3D axis_vec = Vector3D::Zero();
    axis_vec[axis]    = 0;
    return quaterion_from_axis_angle(axis_vec, angle);
}

Quaternion QuaternionHelpers::quaterion_from_average(const Quaternion& arg1, const Quaternion& arg2)
{
    const double w = (arg1.w() + arg2.w()) / 2.0;
    const double x = (arg1.x() + arg2.x()) / 2.0;
    const double y = (arg1.y() + arg2.y()) / 2.0;
    const double z = (arg1.z() + arg2.z()) / 2.0;
    return Quaternion(w, x, y, z);
}

void QuaternionHelpers::add_after(Quaternion& quaternion_to_modify,
                                  const Quaternion& additional_quaternion)
{
    quaternion_to_modify *= additional_quaternion;
}

void QuaternionHelpers::add_before(Quaternion& quaternion_to_modify,
                                   const Quaternion& additional_quaternion)
{
    quaternion_to_modify = additional_quaternion * quaternion_to_modify;
}

bool QuaternionHelpers::no_rotation(const Quaternion& quaterion)
{
    return quaterion.w() == 1.0;
}

Matrix3D QuaternionHelpers::get_direction_cosine_matrix(const Quaternion& quaterion)
{
    Matrix3D DCM;
    get_direction_cosine_matrix(quaterion, DCM);
    return DCM;
}

void QuaternionHelpers::get_direction_cosine_matrix(const Quaternion& quaterion, Matrix3D& DCM)
{
    DCM = quaterion.toRotationMatrix();
}

void QuaternionHelpers::get_voigt_rotation_matrix(const Quaternion& quaterion, VoigtMatrix& VRM)
{
    
    Matrix3D a = get_direction_cosine_matrix(quaterion);
    get_voigt_rotation_matrix(a, VRM);
}

void simple_math::QuaternionHelpers::get_voigt_rotation_matrix(const simple_math::Matrix3D& a,
                                                               VoigtMatrix& VRM)
{
    VRM.resize(6, 6);
    VRM(0, 0) = a(0, 0) * a(0, 0);
    VRM(0, 1) = a(0, 1) * a(0, 1);
    VRM(0, 2) = a(0, 2) * a(0, 2);
    VRM(0, 3) = 2.0 * a(0, 1) * a(0, 2);
    VRM(0, 4) = 2.0 * a(0, 0) * a(0, 2);
    VRM(0, 5) = 2.0 * a(0, 0) * a(0, 1);

    VRM(1, 0) = a(1, 0) * a(1, 0);
    VRM(1, 1) = a(1, 1) * a(1, 1);
    VRM(1, 2) = a(1, 2) * a(1, 2);
    VRM(1, 3) = 2.0 * a(1, 1) * a(1, 2);
    VRM(1, 4) = 2.0 * a(1, 0) * a(1, 2);
    VRM(1, 5) = 2.0 * a(1, 0) * a(1, 1);

    VRM(2, 0) = a(2, 0) * a(2, 0);
    VRM(2, 1) = a(2, 1) * a(2, 1);
    VRM(2, 2) = a(2, 2) * a(2, 2);
    VRM(2, 3) = 2.0 * a(2, 1) * a(2, 2);
    VRM(2, 4) = 2.0 * a(2, 0) * a(2, 2);
    VRM(2, 5) = 2.0 * a(2, 0) * a(2, 1);

    VRM(3, 0) = a(1, 0) * a(2, 0);
    VRM(3, 1) = a(1, 1) * a(2, 1);
    VRM(3, 2) = a(1, 2) * a(2, 2);
    VRM(3, 3) = a(1, 1) * a(2, 2) + a(1, 2) * a(2, 1);
    VRM(3, 4) = a(1, 0) * a(2, 2) + a(1, 2) * a(2, 0);
    VRM(3, 5) = a(1, 0) * a(2, 1) + a(1, 1) * a(2, 0);

    VRM(4, 0) = a(0, 0) * a(2, 0);
    VRM(4, 1) = a(0, 1) * a(2, 1);
    VRM(4, 2) = a(0, 2) * a(2, 2);
    VRM(4, 3) = a(0, 1) * a(2, 2) + a(0, 2) * a(2, 1);
    VRM(4, 4) = a(0, 0) * a(2, 2) + a(0, 2) * a(2, 0);
    VRM(4, 5) = a(0, 0) * a(2, 1) + a(0, 1) * a(2, 0);

    VRM(5, 0) = a(0, 0) * a(1, 0);
    VRM(5, 1) = a(0, 1) * a(1, 1);
    VRM(5, 2) = a(0, 2) * a(1, 2);
    VRM(5, 3) = a(0, 1) * a(1, 2) + a(0, 2) * a(1, 1);
    VRM(5, 4) = a(0, 0) * a(1, 2) + a(0, 2) * a(1, 0);
    VRM(5, 5) = a(0, 0) * a(1, 1) + a(0, 1) * a(1, 0);
}

void QuaternionHelpers::get_voigt_engineering_rotation_matrix(const Quaternion& quaterion,
                                                              VoigtMatrix& VRM)
{
    VRM.resize(6, 6);
    Matrix3D a = get_direction_cosine_matrix(quaterion);

    VRM(0, 0) = a(0, 0) * a(0, 0);
    VRM(0, 1) = a(0, 1) * a(0, 1);
    VRM(0, 2) = a(0, 2) * a(0, 2);
    VRM(0, 3) = a(0, 1) * a(0, 2);
    VRM(0, 4) = a(0, 0) * a(0, 2);
    VRM(0, 5) = a(0, 0) * a(0, 1);

    VRM(1, 0) = a(1, 0) * a(1, 0);
    VRM(1, 1) = a(1, 1) * a(1, 1);
    VRM(1, 2) = a(1, 2) * a(1, 2);
    VRM(1, 3) = a(1, 1) * a(1, 2);
    VRM(1, 4) = a(1, 0) * a(1, 2);
    VRM(1, 5) = a(1, 0) * a(1, 1);

    VRM(2, 0) = a(2, 0) * a(2, 0);
    VRM(2, 1) = a(2, 1) * a(2, 1);
    VRM(2, 2) = a(2, 2) * a(2, 2);
    VRM(2, 3) = a(2, 1) * a(2, 2);
    VRM(2, 4) = a(2, 0) * a(2, 2);
    VRM(2, 5) = a(2, 0) * a(2, 1);

    VRM(3, 0) = 2.0 * a(1, 0) * a(2, 0);
    VRM(3, 1) = 2.0 * a(1, 1) * a(2, 1);
    VRM(3, 2) = 2.0 * a(1, 2) * a(2, 2);
    VRM(3, 3) = a(1, 1) * a(2, 2) + a(1, 2) * a(2, 1);
    VRM(3, 4) = a(1, 0) * a(2, 2) + a(1, 2) * a(2, 0);
    VRM(3, 5) = a(1, 0) * a(2, 1) + a(1, 1) * a(2, 0);

    VRM(4, 0) = 2.0 * a(0, 0) * a(2, 0);
    VRM(4, 1) = 2.0 * a(0, 1) * a(2, 1);
    VRM(4, 2) = 2.0 * a(0, 2) * a(2, 2);
    VRM(4, 3) = a(0, 1) * a(2, 2) + a(0, 2) * a(2, 1);
    VRM(4, 4) = a(0, 0) * a(2, 2) + a(0, 2) * a(2, 0);
    VRM(4, 5) = a(0, 0) * a(2, 1) + a(0, 1) * a(2, 0);

    VRM(5, 0) = 2.0 * a(0, 0) * a(1, 0);
    VRM(5, 1) = 2.0 * a(0, 1) * a(1, 1);
    VRM(5, 2) = 2.0 * a(0, 2) * a(1, 2);
    VRM(5, 3) = a(0, 1) * a(1, 2) + a(0, 2) * a(1, 1);
    VRM(5, 4) = a(0, 0) * a(1, 2) + a(0, 2) * a(1, 0);
    VRM(5, 5) = a(0, 0) * a(1, 1) + a(0, 1) * a(1, 0);
}

void QuaternionHelpers::get_2D_voigt_rotation_matrix(const Quaternion& quaterion, VoigtMatrix& VRM)
{
    VRM.resize(3, 3);
    Matrix3D a = get_direction_cosine_matrix(quaterion);

    VRM(0, 0) = a(0, 0) * a(0, 0);
    VRM(0, 1) = a(0, 1) * a(0, 1);
    VRM(0, 2) = 2.0 * a(0, 1) * a(0, 0);

    VRM(1, 0) = a(1, 0) * a(1, 0);
    VRM(1, 1) = a(1, 1) * a(1, 1);
    VRM(1, 2) = 2.0 * a(1, 1) * a(1, 0);

    VRM(2, 0) = a(0, 0) * a(1, 0);
    VRM(2, 1) = a(0, 1) * a(1, 1);
    VRM(2, 2) = a(0, 1) * a(1, 0) + a(0, 0) * a(1, 1);
}

void QuaternionHelpers::get_2D_voigt_engineering_rotation_matrix(const Quaternion& quaterion,
                                                                 VoigtMatrix& VRM)
{
    VRM.resize(3, 3);
    Matrix3D a = get_direction_cosine_matrix(quaterion);

    VRM(0, 0) = a(0, 0) * a(0, 0);
    VRM(0, 1) = a(0, 1) * a(0, 1);
    VRM(0, 2) = a(0, 1) * a(0, 0);

    VRM(1, 0) = a(1, 0) * a(1, 0);
    VRM(1, 1) = a(1, 1) * a(1, 1);
    VRM(1, 2) = a(1, 1) * a(1, 0);

    VRM(2, 0) = 2.0 * a(0, 0) * a(1, 0);
    VRM(2, 1) = 2.0 * a(0, 1) * a(1, 1);
    VRM(2, 2) = a(0, 1) * a(1, 0) + a(0, 0) * a(1, 1);
}

bool QuaternionHelpers::get_321_euler_sequence_angles(const Quaternion& quaterion,
                                                      std::vector<double>& angles)
{
    // We need the local x, y, z vectors expressed in the unprimed system for the Euler angles,
    // which will tell us how to get to the primed from the unprimed. Expressing the system as:
    // primedXYZ = DCM * unprimedXYZ, where unprimedXYZ = [ 1 0 0 ] and primedXYZ = [ x[0] y[0] z[0]
    // ]
    //               [ 0 1 0 ]                 [ x[1] y[1] z[1] ]
    //               [ 0 0 1 ]                 [ x[2] y[2] z[2] ]
    // Then GlobalXYZ = DCM
    Matrix3D DCM = get_direction_cosine_matrix(quaterion);
    Vector3D x   = DCM.col(0);
    Vector3D y   = DCM.col(1);
    Vector3D z   = DCM.col(2);

    angles.resize(3);
    // See "Analytical Mechanics of Space Systems" by Schaub and Junkins, P 81, Eq 3.34

    if (fabs(fabs(x(2)) - 1.0) < 1.0e-8)
    {
        // if x[2] = 1 or -1, then 3-2-1 sequence is not able to describe the orientation
        return false;
    }

    angles[0] = atan2(x(1), x(0)) * 180.0 / M_PI;
    angles[1] = -asin(x(2)) * 180.0 / M_PI;
    angles[2] = atan2(y(2), z(2)) * 180.0 / M_PI;
    return true;
}

}