#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class Shader {
public:
  // The program ID
  unsigned int ID;

  // Constructor reads and builds the shader
  Shader(const char* vertexPath, const char* fragmentPath);
  // Use/Activate the shader
  void Use();
  // Utility uniform functions
  void SetBool(const std::string &name, bool value) const;
  void SetInt(const std::string &name, int value) const;
  void SetFloat(const std::string &name, float value) const;
private:
  void CheckCompileErrors(unsigned int shader, std::string type);
};

#endif
