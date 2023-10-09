#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"

#include "shapes/sphere.h"
#include "shapes/infiniteplan.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "shaders/normalshader.h"
#include "materials/phong.h"
#include "shaders/directshader.h"





void buildSceneSphere(Camera*& cam, Film*& film,
    std::vector<Shape*>*& objectsList,
    std::vector<PointLightSource>*& lightSourceList)
{
    /* **************************** */
      /* Declare and place the camera */
      /* **************************** */
      // By default, this gives an ID transform
      //  which means that the camera is located at (0, 0, 0)
      //  and looking at the "+z" direction
    Matrix4x4 cameraToWorld;
    double fovDegrees = 60;
    double fovRadians = Utils::degreesToRadians(fovDegrees);
    cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

    /* ************************** */
    /* DEFINE YOUR MATERIALS HERE */
    /* ************************** */
    Material *red_50 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.6, 0.2, 0.2), 50);
    Material* green_50 = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 50);
    Material* blue_50 = new Phong(Vector3D(0.2, 0.3, 0.7), Vector3D(0.2, 0.2, 0.6), 50);


    /* ******* */
    /* Objects */
    /* ******* */
    // Create a heterogeneous list of objects of type shape
    // (some might be triangles, other spheres, plans, etc)
    objectsList = new std::vector<Shape*>;

    // Define and place a sphere
    Matrix4x4 sphereTransform1;
    sphereTransform1 = sphereTransform1.translate(Vector3D(-.75, -0.5, 2 * std::sqrt(2.0)));
    Shape* s1 = new Sphere(0.25, sphereTransform1, green_50);

    // Define and place a sphere
    Matrix4x4 sphereTransform2;
    sphereTransform2 = sphereTransform2.translate(Vector3D(1.25, 0.0, 6));
    Shape* s2 = new Sphere(1.25, sphereTransform2, red_50);

    // Define and place a sphere
    Matrix4x4 sphereTransform3;
    sphereTransform3 = sphereTransform3.translate(Vector3D(1.0, -0.75, 3.5));
    Shape* s3 = new Sphere(0.25, sphereTransform3, blue_50);

    // Store the objects in the object list
    objectsList->push_back(s1);
    objectsList->push_back(s2);
    objectsList->push_back(s3);


    /* ****** */
    /* Lights */
    /* ****** */
    //

    //
    //TIP: When computing Phong Illumination start with one light!!
    // 


    double offset = 3.0;
    lightSourceList = new std::vector<PointLightSource>;

    Vector3D lightPosition1 = Vector3D(-2.0, -0.5, 0.0);
    Vector3D lightPosition2 = Vector3D(2., offset, 3.0);
    Vector3D lightPosition3 = Vector3D(1.0, 0.0, -5.0);
    Vector3D intensity = Vector3D(10, 10, 10); // Radiant intensity (watts/sr)
    PointLightSource pointLS1(lightPosition1, intensity);
    PointLightSource pointLS2(lightPosition2, intensity);
    PointLightSource pointLS3(lightPosition3, intensity);
    lightSourceList->push_back(pointLS1);
    lightSourceList->push_back(pointLS2);
    lightSourceList->push_back(pointLS3);

    
}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
              std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
    for(size_t lin=0; lin<resY; lin++)
    {
        // Show progression
        if ( lin%(resY/sizeBar) == 0)
            std::cout << ".";

        // Inner loop invariant: we have rendered col columns
        for(size_t col=0; col<resX; col++)
        {
            // Compute the pixel position in NDC
            double x = (double)(col + 0.5) / resX;
            double y = (double)(lin + 0.5) / resY;

            // Generate the camera ray
            Ray cameraRay = cam->generateRay(x, y);

            // Compute ray color according to the used shader
            Vector3D pixelColor = shader->computeColor( cameraRay, *objectsList, *lightSourceList );

            // Store the pixel color
            film->setPixelValue(col, lin, pixelColor);
        }
    }
}


//------------TASK 1---------------------//
void PaintImage(Film* film)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main Image Loop
    for (size_t lin = 0; lin < resY; lin++)
    {
        // Show progression
        if (lin % (resY / sizeBar) == 0)
            std::cout << ".";

        for (size_t col = 0; col < resX; col++)
        {
            Vector3D pixel_color = Vector3D((double)col / resX, (double)lin / resY, 0);
            film->setPixelValue(col, lin, pixel_color);

        }
    }
}

int main()
{
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RT-ACG - Ray Tracer for \"Advanced Computer Graphics\"" << separator << std::endl;

    // Create an empty film
    Film *film;
    film = new Film(720, 512);

    // Declare the Intersection Shader
    Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
    Vector3D intersectionColor(1, 0, 0);
    double maxDist = 8;
    
    // Declare the Depth Shader
    Vector3D depthColor(1, 0, 0);

    Shader* intShader = new IntersectionShader(intersectionColor, bgColor); //TASK 2
    Shader* depthShader = new DepthShader(depthColor, maxDist, bgColor); //TASK 3
    Shader* normalShader = new NormalShader(depthColor, maxDist, bgColor); //TASK 4
    Shader* directShader = new DirectShader(depthColor, maxDist, bgColor); //TASK 5


    // Build the scene---------------------------------------------------------
    // 
    // Declare pointers to all the variables which describe the scene
    Camera* cam;
    std::vector<Shape*>* objectsList;
    std::vector<PointLightSource>* lightSourceList;
    //Create Scene Geometry and Illumiantion
    buildSceneSphere(cam, film, objectsList, lightSourceList);

    //---------------------------------------------------------------------------

    //Paint Image ONLY TASK 1
    PaintImage(film);

    // Launch some rays! TASK 2,3,...
    // 
    //raytrace(cam, intShader, film, objectsList, lightSourceList); //TASK 2
    //raytrace(cam, depthShader, film, objectsList, lightSourceList); //TASK 3
    //raytrace(cam, normalShader, film, objectsList, lightSourceList); //TASK 4
    raytrace(cam, directShader, film, objectsList, lightSourceList); //TASK 5
    

    // Save the final result to file
    std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
    film->save();

    std::cout << "\n\n" << std::endl;
    return 0;
}
