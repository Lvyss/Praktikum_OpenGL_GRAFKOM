#version 330 core
out vec4 FragColor;
in vec3 ourPosition; // MENERIMA POSISI DARI VERTEX SHADER
void main()
{
 // MENGGUNAKAN POSISI SEBAGAI WARNA
 FragColor = vec4(ourPosition, 1.0);
}