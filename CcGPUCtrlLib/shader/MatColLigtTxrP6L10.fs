#version 400 core

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 


struct lightProp {
    int  lightType;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
    
    float cutOff;
    float outerCutOff;
};

// Light definition, which defines its ....
struct lightDef {
    int  indxToProp;    // Index to above light property 
    vec3 position;      // For Point and Spot light (No use for Direction)
    vec3 direction;     // For Direction and Spot light (No use for Point)
};

#define MAX_LIGHT_PROP 6
#define MAX_LIGHTS     10

#define LIGHT_TYPE_DIRECTION     1
#define LIGHT_TYPE_POINT         2
#define LIGHT_TYPE_SPOT          3

// Texture Type
#define TEXTURE_NO       0  // No Texture
#define TEXTURE_ONLY     1  // Only Texture applied
#define TEXTURE_PLUS_MAT 2 // Texture Plus Material color
#define TEXTURE_MUL_MAT  3 // Texture Multiply with Material color

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform int numOfLightsU;   // Number of Lights
uniform int lightTypeU;
uniform float alphaU;

uniform vec3 glblIluminU;   // Global Illumination
uniform vec3 viewPosU;      // Camera view Position

// Structure implementation of above Struct
uniform Material     materialU;

uniform lightProp    lightPropAry[MAX_LIGHT_PROP];    // Array of Light property
uniform lightDef     lightDefAry[MAX_LIGHTS];       // Array of Lights 

//Texture sampler Type uniform
uniform sampler2D smpl2Dtxr;

// Texture Type.
uniform int txrTypeU;   // No Txr, Only Txr,  Mat. Color * Txr,  Mat. Color + Txr 


// function prototypes

vec3 CalcDirLight(lightProp lgtPropP, vec3 ltDirP, vec3 normalP, vec3 viewDirP);
vec3 CalcPointLight(lightProp lgtPropP, vec3 ltPosP, vec3 normalP, vec3 viewDirP, vec3 fragPosP);
vec3 CalcSpotLight(lightProp lgtPropP, vec3 ltPosP, vec3 ltDirP, vec3 normalP, vec3 viewDirP, vec3 fragPosP);


void main()
{    
    int i;
    int iP;
    int lightTypeL;

    vec3 lgtPosL;
    vec3 lgtDirL;
    
    vec3 NormalL = normalize(Normal);
    vec3 viewDirL = normalize(viewPosU - FragPos);  // view direction is Camera position w.r.t. Fragment position
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    
    // Init Result to glblIluminU
   vec3 result = vec3(glblIluminU.r, glblIluminU.g, glblIluminU.b);

   for(i = 0; i < numOfLightsU; i++)
    {
        lgtPosL = lightDefAry[i].position;
        lgtDirL = lightDefAry[i].direction;
               
        iP = lightDefAry[i].indxToProp;
        lightTypeL = lightPropAry[iP].lightType;

        if (lightTypeL  == LIGHT_TYPE_DIRECTION)   result += CalcDirLight(lightPropAry[iP], lgtDirL, NormalL, viewDirL);
        if (lightTypeL == LIGHT_TYPE_POINT)      result += CalcPointLight(lightPropAry[iP], lgtPosL, NormalL, viewDirL, FragPos);    
        if (lightTypeL == LIGHT_TYPE_SPOT)       result += CalcSpotLight(lightPropAry[iP], lgtPosL, lgtDirL, NormalL, viewDirL, FragPos);  
     }
   
   // Apply texture 
   // Get teture color for this interpolated texture coordinate.
   //vec3 texColorL =  vec3(texture(smpl2Dtxr, TexCoord));
   //result   *= texColorL;

   vec4 FragColorL;

   if (txrTypeU == TEXTURE_ONLY)
   {
      // result has only the light component (with out Material) for this fragment.
      FragColorL = vec4(result, alphaU) * texture(smpl2Dtxr, TexCoord);
     // FragColorL = texture(smpl2Dtxr, TexCoord);

   }else if (txrTypeU == TEXTURE_PLUS_MAT)
   {
      FragColorL = vec4(result, alphaU) + texture(smpl2Dtxr, TexCoord);

   }else if (txrTypeU == TEXTURE_MUL_MAT)
   {
      FragColorL = vec4(result, alphaU) * texture(smpl2Dtxr, TexCoord);

   }else
   {
   //  no texture ???
      FragColorL = vec4(result, alphaU);
   }

   FragColor = FragColorL;

}


//    /*  *****      Sub routines  ****** */

// Sub 1: calculates the color when using a directional light.
vec3 CalcDirLight(lightProp lgtPropP, vec3 ltDirP, vec3 normalP, vec3 viewDirP)
{
  vec3 ambientL, diffuseL, specularL;

    // ltDirP is towards the light source. i.e. (Position - 0) i.e. cener towards  Sun
    vec3 lightDirL = normalize(ltDirP);
    // diffuse shading
    float diffL = max(dot(normalP, lightDirL), 0.0);

    // specular shading  (lightDirL should be -Ve, as it is incident ray. 
    // i.e. from light source. But gives spot in other direction)
    vec3 reflectDirL = reflect(lightDirL, normalP);     // -lightDirL ??? gives problem !
    float specL = pow(max(dot(viewDirP, reflectDirL), 0.0), materialU.shininess);
    // Pl. note above specular gives a dot in opposite direction of light source.
    // This problem needs to be resolved. ???
   

   if (txrTypeU == TEXTURE_ONLY) 
   {
    ambientL = lgtPropP.ambient;
    diffuseL = lgtPropP.diffuse * diffL;
    specularL = lgtPropP.specular * specL;

   }else
   {
    // combine results
    ambientL = lgtPropP.ambient * materialU.ambient;
    diffuseL = lgtPropP.diffuse * (diffL * materialU.diffuse);
    specularL = lgtPropP.specular * (specL * materialU.specular);
   }

    return (ambientL + diffuseL + specularL);
}


// Sub2: calculates the color when using a point light.
vec3 CalcPointLight(lightProp lgtPropP, vec3 ltPosP, vec3 normalP, vec3 viewDirP, vec3 fragPosP)
{
  vec3 ambientL, diffuseL, specularL;

    // Light direction = light position w.r.t.  Fragment position.
    vec3 lgtFrgDirL = normalize(ltPosP - fragPosP);
    // diffuse component
    float diffL = max(dot(normalP, lgtFrgDirL), 0.0);

    // specular shading  (lightDirL should be -Ve,  as above im Dir Light)
    vec3 reflectDirL = reflect(lgtFrgDirL, normalP);    // ???
    float specL = pow(max(dot(viewDirP, reflectDirL), 0.0), materialU.shininess);

    // attenuation
    float distanceL = length(ltPosP - fragPosP);
    float attenuationL = 1.0 / (lgtPropP.constant + lgtPropP.linear * distanceL + lgtPropP.quadratic * (distanceL * distanceL));    
    
    // combine results
    if (txrTypeU == TEXTURE_ONLY) 
    {
     ambientL = lgtPropP.ambient;
     diffuseL = lgtPropP.diffuse * diffL;
     specularL = lgtPropP.specular * specL;

    }else
    {
     // combine results
     ambientL = lgtPropP.ambient * materialU.ambient;
     diffuseL = lgtPropP.diffuse * (diffL * materialU.diffuse);
     specularL = lgtPropP.specular * (specL * materialU.specular);
    }
    
    ambientL *= attenuationL;
    diffuseL *= attenuationL;
    specularL *= attenuationL;
    return (ambientL + diffuseL + specularL);
}


// Sub3: calculates the color when using a spot light.
vec3 CalcSpotLight(lightProp lgtPropP, vec3 ltPosP, vec3 ltDirP, vec3 normalP, vec3 viewDirP, vec3 fragPosP)
{
  vec3 ambientL, diffuseL, specularL;

    vec3 lgtFrgDirL = normalize(ltPosP - fragPosP);

    // diffuse shading
    float diffL = max(dot(normalP, lgtFrgDirL), 0.0);
    
    // specular shading  (lightDirL should be -Ve,  as above im Dir Light)
    vec3 reflectDirL = reflect(lgtFrgDirL, normalP);   // ???
    float specL = pow(max(dot(viewDirP, reflectDirL), 0.0), materialU.shininess);
    
    // attenuation
    float distanceL = length(ltPosP - fragPosP);
    float attenuationL = 1.0 / (lgtPropP.constant + lgtPropP.linear * distanceL + lgtPropP.quadratic * (distanceL * distanceL));    
    
    // spotlight intensity
    float thetaL     = dot(lgtFrgDirL, normalize(-ltDirP)); 
    float epsilonL   = lgtPropP.cutOff - lgtPropP.outerCutOff;
    float intensityL = clamp((thetaL - lgtPropP.outerCutOff) / epsilonL, 0.0, 1.0);


    // combine results
    if (txrTypeU == TEXTURE_ONLY) 
    {
     ambientL = lgtPropP.ambient;
     diffuseL = lgtPropP.diffuse * diffL;
     specularL = lgtPropP.specular * specL;

    }else
    {
     // combine results
     ambientL = lgtPropP.ambient * materialU.ambient;
     diffuseL = lgtPropP.diffuse * (diffL * materialU.diffuse);
     specularL = lgtPropP.specular * (specL * materialU.specular);
    }

  //  if (intensityL < 0.001) intensityL = 0.1;

    ambientL  *= attenuationL * intensityL;
    diffuseL  *= attenuationL * intensityL;
    specularL *= attenuationL * intensityL;
    return (ambientL + diffuseL + specularL);
}