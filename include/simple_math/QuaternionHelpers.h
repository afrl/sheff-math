#pragma once

#include "utility/Exceptions.h"

#include "simple_math/EigenDefines.h"

#include <array>
#include <vector>

namespace simple_math
{
	/// \brief
	/// A static class that provides many Quaterion helper methods needed in a FEA code.
	///
	/// Primarily, quaternions will be used in each of the operations, for several
	/// reasons. As observed by Ross McLendon from Texas A&M, the advantages include:
	/// 1. Their application to rotations is well-understood
	/// 2. They do not exhibit any singularities
	/// 3. They represent a convenient, compact method for storing an arbitrary rotation
	/// (four doubles)
	/// 4. They reduce the number and complexity of operations to perform a number of basic
	/// rotation functions
	/// - forming a direction cosine matrix(DCM) does not require any trig - function calls
	/// - combining successive rotations takes the form of a 4x4 * 4x1 matrix - vector multiply
	/// Furthermore, Eigen has robust support for quaternion operations.
	///
	/// This class builds on top Eigen to provide conversions, transform Voigt vectors
	/// and matrices, interpolate quaternions, etc.
	/// It should be noted that the quaternions inside the FEA code generally transform from
	/// the local/material coordinate system to the global coordinate system.  The methods here
	/// are generic, but this means that in the FEA code, the local coordinate system is typically
	/// the unprimed coordinate system and the global is generally the primed coordinate system.
	///
	/// An statement originally made by Ross:
	/// A brief description of defining rotations using quaternions:
	/// Assume you wish to describe a tensor in a coordinate system which is obtained through a
	/// single rotaiton about the axis defined by a unit vector <e1, e2, e3> by angle theta(the
	/// coordinate system is being rotated about the fixed tensor).The quaternion describing
	/// this rotation is defined in the following way :
	/// b0 = cos(theta / 2)
	/// b1 = e1 * sin(theta / 2)
	/// b2 = e2 * sin(theta / 2)
	/// b3 = e3 * sin(theta / 2)
	///
	/// It is noted that b0 ^ 2 + b1 ^ 2 + b2 ^ 2 + b3 ^ 2 = 1 for all rotations.
	///
	/// More information, including how to obtain the rotation tensor from a quaternion and how to
	/// combine a sequence of quaternion rotations, can be found in Schaub & Junkins "Analytical
	/// Mechanics of Space Systems", Pp. 95-101
	class QuaternionHelpers
	{
	public:
		/////////////////////////
		// Creation/conversion //
		/////////////////////////

		/// Constructes a quaternion from the classical quaternion parameters.
		/// This is helpful since Eigen actually uses a weird order for the parameters in the constructor.
		/// This method provides a more common order.
		static Quaternion quaterion_from_parameters(const double b0, const double b1, const double b2, const double b3);
		/// Create a quaternion that defines a rotation from an unprimed coordinate system to a primed coordinate system.
		/// The rotation is about some arbitrary vector defined in the unprimed coordinate system, though it should be noted
		/// that the vector ends up being the same in both coordinate systems.  The input angle is the angle that the
		/// unprimed coordinate system is rotated about the vector to obtain the primed coordiante system.
		/// The sign of the input angle is determined by the right hand rule.
		static Quaternion quaterion_from_axis_angle(const Vector3D axis, const double angle);
		/// Create a quaternion that defines a rotation from an unprimed coordinate system to a primed coordinate system.
		/// Similar to quaterion_from_axis_angle(const Vector3D axis, const double angle), but this method rotates about
		/// an unprimed coordinate system axis (x, y, z).
		static Quaternion quaterion_from_axis_angle(const int axis, const double angle);
		/// Computes the average of two quaternions (specifically the parameters)
		/// This is primarily used in converting a linear mesh to a quadratic mesh
		/// In which case, the orientations of the midside nodes will be the average of the two corner
		/// nodes that the midside node lies between.
		static Quaternion quaterion_from_average(const Quaternion & arg1, const Quaternion & arg2);

		//////////////////////////
		// Composite Operations //
		//////////////////////////

		/// Modifies a quaternion so that it is eqivalent to a composite rotation consisting of
		/// performing quaternion_to_modify then the additional_quaternion
		static void add_after(Quaternion & quaternion_to_modify, const Quaternion & additional_quaternion);

		/// Modifies a quaternion so that it is eqivalent to a composite rotation consisting of
		/// performing additional_quaternion then the quaternion_to_modify
		static void add_before(Quaternion & quaternion_to_modify, const Quaternion & additional_quaternion);

		///////////////////
		// Interpolation //
		///////////////////

		/// Calculate the rotation at a point given by applying Lagrange interpolation to
		/// quaternion parameters and re-normalizing.  Note: this is not the most accurate method,
		/// but it is cheap.  Other methods of interpolation should be considered later.
		template<typename Container>
		static void interpolate_rotation(
			const simple_math::GeneralVector & shape_function_values,
			const Container & nodal_quaternions);

		/////////////////
		// Information //
		/////////////////

		/// Tests if a quaternion would result in no rotation
		static bool no_rotation(const Quaternion & quaterion);

		///////////////////////
		// Rotation Matrices //
		///////////////////////

		/// Obtain the direction cosine matrix from quaternion
		/// Note: matrix transforms from unprimed to primed
		/// i.e. $ v_i' = a_{ij} * v_j, where a_{ij} is the cosine matrix
		static Matrix3D get_direction_cosine_matrix(const Quaternion & quaterion);

		/// Obtain the direction cosine matrix from quaternion
		/// Faster direction cosine, modify pre-allocated matrix (no copy required)
		/// Note: matrix transforms from unprimed to primed
		/// i.e. $ v_i' = a_{ij} * v_j, where a_{ij} is the cosine matrix
		static void get_direction_cosine_matrix(const Quaternion & quaterion, Matrix3D & DCM);

		/// Populate 6x6 matrix which will rotate a 2nd order tensor (i.e. stress) in Voigt notation
		/// Note: Ordering of voigt notation is standard (xx, yy, zz, yz, xz, xz)
		/// Note: matrix transforms from unprimed to primed
		/// i.e. $ v_i' = a_{ij} * v_j, where a_{ij} is the rotation matrix and v is a Voigt vector
		static void get_voigt_rotation_matrix(const Quaternion & quaterion, VoigtMatrix & VRM);

		///Populate 6x6 matrix which will rotate a Voigt engineering strain
		/// Note: Ordering of voigt notation is standard (xx, yy, zz, yz, xz, xz)
		/// Note: matrix transforms from unprimed to primed
		/// i.e. $ v_i' = a_{ij} * v_j, where a_{ij} is the rotation matrix and v is a Voigt vector
		static void get_voigt_engineering_rotation_matrix(const Quaternion & quaterion, VoigtMatrix & VESRM);

		/// Populate 3x3 matrix which will rotate a 2nd order tensor (i.e. stress) in Voigt notation
		/// Note: matrix transforms from unprimed to primed
		/// i.e. $ v_i' = a_{ij} * v_j, where a_{ij} is the rotation matrix and v is a Voigt vector
		static void get_2D_voigt_rotation_matrix(const Quaternion & quaterion, VoigtMatrix & VRM);

		/// Populate 3x3 matrix which will rotate a Voigt engineering strain
		/// Note: matrix transforms from unprimed to primed
		/// i.e. $ v_i' = a_{ij} * v_j, where a_{ij} is the rotation matrix and v is a Voigt vector
		static void get_2D_voigt_engineering_rotation_matrix(const Quaternion & quaterion, VoigtMatrix & VESRM);

		/// Determine a 3-2-1 sequence of rotations that are equivalent to the quaternion
		/// Return false if orientation cannot be attained using 3-2-1 sequence
		/// NOTE: TODO this needs to be cheked!!!
		static bool get_321_euler_sequence_angles(const Quaternion & quaterion, std::vector<double> & angles);
	};

	//////////////////////////
	// Template definitions //
	//////////////////////////

	template<typename Container>
	inline void QuaternionHelpers::interpolate_rotation(const simple_math::GeneralVector & shape_function_values, const Container & nodal_quaternions)
	{
		const double w = x = y = z = 0.0;

		for (int i = 0; i < shape_function_values.rows(); ++i) {
			w += shape_function_values(i) * nodal_quaternions[i].w();
			x += shape_function_values(i) * nodal_quaternions[i].x();
			y += shape_function_values(i) * nodal_quaternions[i].y();
			z += shape_function_values(i) * nodal_quaternions[i].z();
		}

		Quaternion interpolated_quaternion(w, x, y, z);
		interpolated_quaternion.normalize();
		return interpolated_quaternion;
	}

}