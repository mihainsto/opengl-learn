// Shader-ul de varfuri  

#version 400


in vec4 in_Position;
in vec4 in_Color;

out vec4 gl_Position;
out vec4 ex_Color;
uniform mat4 myMatrix;
uniform int poligonD;



void main(void)
{


    if(poligonD == 0){
        gl_Position = myMatrix*in_Position;
        ex_Color = in_Color;
    }
    if(poligonD == 1){
        gl_Position = vec4(in_Position.x * 0.0012, in_Position.y * 0.0012, 0, 1);
        ex_Color = in_Color;
    }
}
