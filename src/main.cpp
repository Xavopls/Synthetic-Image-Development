#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>
#include <math.h>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"
#include "shapes/sphere.h"
#include "cameras/ortographic.h"
#include "cameras/perspective.h"


#define PI = 3.141592653

void transformationsExercise()
{
    /* *************** */
    /* TRANSFORMATIONS */
    /* *************** */
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";

    std::cout << separatorStar << "Construct and visualize transforms" << separatorStar << std::endl;

    // Identity Matrix
    Matrix4x4 idMat; // By default, a matrix4x4 is initialized with the identity matrix
    std::cout << "The content of matrix idMat is: \n" << std::endl;
    std::cout << idMat << separator << std::endl;

    // Translation Matrix
    double deltaX, deltaY, deltaZ;
    deltaX = 1; deltaY = -2; deltaZ = 3;
    Vector3D delta(deltaX, deltaY, deltaZ);
    Matrix4x4 translationMat = Matrix4x4::translate(delta);
    std::cout << "The content of matrix translationMat is: \n" << std::endl;
    std::cout << translationMat << separator << std::endl;

    // Scale Matrix
	double sX, sY, sZ;
	sX = 2; sY = 1; sZ = -1;
	Vector3D scalingVector(sX, sY, sZ);
	Matrix4x4 scaleMatrix = Matrix4x4::scale(scalingVector);

    std::cout << "The content of matrix scaleMatrix is: \n" << std::endl;
    std::cout << scaleMatrix << separator << std::endl;

    // Rotate around X Matrix
    double angleInDegrees = 60;
    double thetaInRadians = Utils::degreesToRadians(angleInDegrees);
	Vector3D rotationVector(1, 0, 0);
	Matrix4x4 rotateXMatrix = Matrix4x4::rotate(thetaInRadians, rotationVector);

    std::cout << "The content of matrix rotateXMatrix is: \n" << std::endl;
    std::cout << rotateXMatrix << separator << std::endl;

    // Rotate around an arbitrary axis Matrix

	double angleInDegrees2 = 30;
	double thetaInRadians2 = Utils::degreesToRadians(angleInDegrees2);
	Vector3D rotationVector2(1, 1, 1);
	Matrix4x4 rotateArtitraryAxisMatrix = Matrix4x4::rotate(thetaInRadians2, rotationVector2);
    
    std::cout << "The content of matrix rotateArtitraryAxisMatrix is: \n" << std::endl;
    std::cout << rotateArtitraryAxisMatrix << separator << std::endl;

    // Transposed and Inversion
    std::cout << separatorStar << "Inverting and Transposing a Matrix" << separatorStar << std::endl;
    Matrix4x4 translationMatTransposed;
    translationMat.transpose(translationMatTransposed);
    std::cout << "The transpose of matrix \n\n" << translationMat << "\n is \n\n"
              << translationMatTransposed << separator << std::endl;

    Matrix4x4 inverseTranslationMat;
	translationMat.inverse(inverseTranslationMat);
	std::cout << "The inverse of matrix \n\n" << translationMat << "\n is \n\n" << inverseTranslationMat << std::endl;
    std::cout << "And their multiplication should thus give the identity matrix:\n\n";
    
	// TxT^(-1)=Id
	Matrix4x4 VerificationMat = translationMat * inverseTranslationMat;

	std::cout<< VerificationMat << "\n\n"  << std::endl;
	



    // Combine here some transforms, and visualize the result
    std::cout << separatorStar << "Combine transforms and visualize the result" << separatorStar << std::endl;

    Matrix4x4 scaleTranslate = scaleMatrix*translationMat;
    std::cout << "The content of matrix scaleTranslate is: \n" << std::endl;
    std::cout << scaleTranslate << separator << std::endl;

    Matrix4x4 translateScale = translationMat*scaleMatrix;
    std::cout << "The content of matrix translateScale is: \n" << std::endl;
    std::cout << translateScale << separator << std::endl;
}

void normalTransformExercise()
{
    std::string separator = "\n----------------------------------------------\n";

    // NOTE: points, vectors and normals are represented, in this framework, using
    //  the Vector3D class. It is up to the programmer to take care of whether the
    //  Vector3D is representing a point, a vector or a normal. For example, when
    //  applying a transform using a Matrix4x4, the programmer must choose which
    //  "applyTransform" method of the namespace Matrix4x4 should be used.

    Vector3D v(1, 1, 0);
    Vector3D n(-1.0/sqrt(2), 1.0/sqrt(2), 0);
    Matrix4x4 S = Matrix4x4::scale(Vector3D(2, 1, 1));
	

    std::cout << "Vector v = " << v << "\n" << std::endl;
    std::cout << "Normal n = " << n << "\n" << std::endl;
    std::cout << "Scale Matrix: \n" << std::endl;
    std::cout << S << std::endl;

    Vector3D vTransformed = S.transformVector(v);
    std::cout << "Vector v\' = " << vTransformed << "\n" << std::endl;

	Vector3D nTransformed = S.transformVector(n);
	std::cout << "Vector n\' = " << nTransformed << "\n" << std::endl;

	double nxvdotprod = dot(nTransformed, vTransformed);
	std::cout << "dot (n',v') = " << nxvdotprod << "\n" << std::endl;

	Matrix4x4 transposedS;
	S.transpose(transposedS);
	Matrix4x4 inverseTransposedS;
	transposedS.inverse(inverseTransposedS);
	
	Vector3D correctN = inverseTransposedS.transformVector(n);
	std::cout << "New Vector n\' = " << correctN << "\n" << std::endl;

	double correctNxVdotprod = dot(correctN, vTransformed);
	std::cout << "New dot product = " << correctNxVdotprod << "\n" << std::endl;



}

void paintingAnImageExercise()
{
    // Define the film (i.e., image) resolution
    size_t resX, resY;
    resX = 512;
    resY = 512;
    Film film(resX, resY);

    for(unsigned int col = 0; col < resX; col++)
        {
			
            for(unsigned int row = 0; row < resY; row++)
            {
                Vector3D color((col+0.5) / resX, (row + 0.5) / resY, 0);

                film.setPixelValue(col, row, color);

            }
        }


    // Save the final result to file
    film.save();
	
	
}

void filteringAnImageExercise()
{
    // Create two instances of the film class with the same resolution
    int resX, resY;
    resX = 512;
    resY = 512;
    Film f1(resX, resY);
    Film f2(resX, resY);

    // Create the original image
    //  Draw a circle centered at centerX, centerY (in pixels, image space)
    //   and with ray r (also in pixels)
    int centerX = resX / 2;
    int centerY = resY / 2;
    int r = std::min(centerX, centerY)/2;

    for(int lin=0; lin<resX; lin++)
    {
        for(int col=0; col<resY; col++)
        {
            // Use the equation of the sphere to determine the pixel color
            if( (lin-centerX)*(lin-centerX) + (col-centerY)*(col-centerY) < r*r )
                f1.setPixelValue(col, lin, Vector3D(1, 1, 0));
        }
    }

    // Filter-related variables
    // Declare here your filter-related variables
    // (e.g., FILTER SIZE)
	const int filterSizeX = 3;
	const int filterSizeY = 3;
	double sigma;
	
	double filter[filterSizeX][filterSizeY];

	for ( int i = 0; i < filterSizeX;  i++){

		for (int j = 0; j < filterSizeY; j++)
		{
			double distance = sqrt((i - 1)*(i - 1) + (j - 1)*(j - 1));
			filter[i][j] = 1 / (2*3.141516*sigma*sigma)*exp(-distance / 2*sigma*sigma);
		}

	}
	


    // Implement here your image filtering algorithm
	for (int lin = 0; lin < resX; lin++)
	{
		for (int col = 0; col < resY; col++)
		{
			
			filter[lin][col] = f1.getPixelValue(lin,col);


		}
	}

    // DO NOT FORGET TO SAVE YOUR IMAGE!
	f2.save();
}

void completeSphereClassExercise()
{
    // Make your intersection tests here
    // (....)
}

void eqSolverExercise()
{
    EqSolver solver;
    rootValues roots;

    double A, B, C;

    // (...)

	bool hasRoots = true;
    //bool hasRoots = solver.rootQuadEq(A, B, C, roots);

    if(!hasRoots)
    {
        std::cout << "Equation has no real roots!" << std::endl;
    }
    else
    {
        // SHOW THE SOLUTIONS OF THE EQUATION
        // (...)
    }
}

void raytrace()
{
    // Define the film (i.e., image) resolution
    size_t resX, resY;
    resX = 512;
    resY = 512;
    Film film(resX, resY);

    /* ******************* */
    /* Orthographic Camera */
    /* ******************* */
    Matrix4x4 cameraToWorld; // By default, this gives an ID transform
                             // meaning that the camera space = world space
    OrtographicCamera camOrtho(cameraToWorld, film);

    /* ******************* */
    /* Perspective Camera */
    /* ******************* */
    double fovRadians = Utils::degreesToRadians(60);
    PerspectiveCamera camPersp(cameraToWorld, fovRadians, film);

    // Save the final result to file
    film.save();
}

int main()
{
    std::string separator = "\n----------------------------------------------\n";

    std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

    // ASSIGNMENT 1
    //transformationsExercise();//ok
    //normalTransformExercise();//ok
    paintingAnImageExercise();
    //filteringAnImageExercise();

    // ASSIGNMENT 2
    //eqSolverExercise();
    //completeSphereClassExercise();
    //raytrace();

    std::cout << "\n\n" << std::endl;
    return 0;
}
