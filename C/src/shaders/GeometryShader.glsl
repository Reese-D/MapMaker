#version 150
layout(triangles) in;
layout(line_strip, max_vertices=14) out;

uniform float normal_length = 1.0;

in Vertex
{
  vec4 normal;
  vec4 color;
} vertex[];

out vec4 vertex_color;

void main()
{
  int i;
  
  //4 points to draw our normal triangles
  //
  for(i=0; i<gl_in.length(); i++)
  {
    vec3 P = gl_in[i].gl_Position.xyz;
    
    gl_Position =  vec4(P, 1.0);
    vertex_color = vertex[i].color;
    EmitVertex();
  }

  gl_Position =  vec4(gl_in[0].gl_Position.xyz, 1.0);
  vertex_color = vertex[0].color;
  EmitVertex();
  EndPrimitive();    

  //------ 3 lines for the 3 vertex normals
  vec3 Normals = vec3(0.0, 0.0, 0.0);
  for(i=0; i<gl_in.length(); i++)
  {
    vec3 P = gl_in[i].gl_Position.xyz;
    vec3 N = vertex[i].normal.xyz;
    Normals += N;
    gl_Position = vec4(P, 1.0);
    vertex_color = vec4(1.0, 0.0, 1.0, 1.0);
    EmitVertex();
    
    gl_Position = vec4(P + N * normal_length, 1.0);
    vertex_color = vec4(1.0, 0.0, 0.0, 1.0);
    EmitVertex();
    
    EndPrimitive();
  }
  //------ One line for the face normal
  
  vec3 P0 = gl_in[0].gl_Position.xyz;
  vec3 P1 = gl_in[1].gl_Position.xyz;
  vec3 P2 = gl_in[2].gl_Position.xyz;
  
  vec3 V0 = P0 - P1;
  vec3 V1 = P2 - P1;
  
  vec3 N = cross(V1, V0);
  N = normalize(N);
  
  // Center of the triangle
  vec3 P = (P0+P1+P2) / 3.0;
  
  gl_Position =  vec4(P, 1.0);
  vertex_color = vec4(1, 1, 0, 1);
  EmitVertex();
  
  gl_Position =  vec4(P + N * normal_length, 1.0);
  vertex_color = vec4(1, 1, 0, 1);
  EmitVertex();
  EndPrimitive();

  //sum of our normals from the center
  vec3 C = vec3(P.x +0.1, P.y, P.z);
  Normals = normalize(Normals);
  gl_Position =  vec4(P, 1.0);
  vertex_color = vec4(1, 0, 1, 1);
  EmitVertex();
  
  gl_Position =  vec4(P + Normals * normal_length, 1.0);
  vertex_color = vec4(1, 0, 1, 1);
  EmitVertex();
  EndPrimitive();
  
}

