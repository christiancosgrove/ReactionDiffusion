#include "MainGame.h"
#include "GLManager.h"
#include <math.h>
#include <random>
#include <SFML\Graphics.hpp>

#include <iostream>
#include <unordered_map>

const float M_PI = 3.1415926535f;
const float M_PI_2 = 3.1415926535f / 2;
const float M_2_PI = 3.1415926535f * 2;
static const GLfloat g_quad_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
};

std::mt19937 rng;

float unirand() {
	std::uniform_real_distribution<float> dist(0, 1);
	return dist(rng);
}


std::string exeDir() {
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);

	std::wstring wstr(path);
	std::string str(wstr.begin(), wstr.end());

	str = str.substr(0, str.find_last_of('\\') + 1);

	return str;
}

MainGame::MainGame() : 
	gl(exeDir() + "..\\ReactionDiffusion\\update.glsl", exeDir() + "..\\ReactionDiffusion\\vshader.glsl")
{
	gl.AddProgram(exeDir() + "..\\ReactionDiffusion\\fshader_draw.glsl", exeDir() + "..\\ReactionDiffusion\\vshader.glsl");
	
	GLuint vbo, vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	initFrame(gl);

	gl.Programs[1].Use();



	/*solver = std::unique_ptr<Solver>(new Solver(gl));

	std::vector<unsigned char> testtex;
	testtex.reserve(3 * 800 * 600);
	for (int i = 0; i < 600; i++) for (int j = 0; j < 800; j++) {
		testtex.push_back(128);
	}

	glActiveTexture(GL_TEXTURE0);
	GLuint t;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB,
		GL_UNSIGNED_BYTE, &testtex[0]);*/

	//glUniform1i(gl.programs[0].GetUniformLocation("tex"), 0);
}



MainGame::~MainGame()
{
}

bool paused = false;
bool del = false;

glm::ivec2 captureStartLocation;

void MainGame::Update(sf::Window& window)
{
	auto mousePos = sf::Mouse::getPosition(window);
	glm::vec2 norm(
		(float)mousePos.x / window.getSize().x,
		1 - (float)mousePos.y / window.getSize().y
	);

	gl.Programs[0].Use();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		gl.Programs[0].SetVector2("brush", norm);
	else
		gl.Programs[0].SetVector2("brush", glm::vec2(-1, -1));

	del = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}


int permutation = 0;
void MainGame::Draw()
{
	glViewport(0, 0, width, width);
	gl.Programs[0].SetBool("paused", paused);
	gl.Programs[0].SetBool("delete", del);
	for (int i = 0; i < 100; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		gl.Programs[0].Use();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[(permutation + 1) % 2], 0);
		gl.Programs[0].SetTexture("tex", permutation % 2);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
		permutation++;
	}

	glViewport(0, 0, wwidth, wheight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	gl.Programs[1].Use();
	gl.Programs[1].SetTexture("tex", (permutation + 1)%2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}


void createWavePacket(std::vector<float>& pixels, std::vector<float>& pixelsPrev, int width, int height, float x, float y, float radius, float frequency, float theta)
{
	for (int i = 0; i<width; i++)
	{
		for (int j = 0; j<height; j++)
		{
			float dx = (float)i / width - x;
			float dy = (float)j / height - y;


			const float high = 0.2;
			const float low = 0.1;
			const float n = frequency;


			const float st = sin(theta);
			const float ct = cos(theta);

			float xdisp = dx*ct + dy*st;
			float ydisp = sqrt((dx - xdisp * ct)*(dx - xdisp * ct) + (dy - xdisp * st)*(dy - xdisp * st));
			float f = 0.1*exp(-(xdisp*xdisp + ydisp*ydisp) / 2 / (radius*radius)) / radius / (sqrt(2 * M_PI));


			pixels[3 * (i + j*width) + 0] += f*(1 + cos(n*(dy*st + dx * ct)*M_2_PI));
			pixelsPrev[3 * (i + j*width) + 0] += f*(1 + cos(n*(dy*st + dx * ct)*M_2_PI + 0.3*sqrt(0.4) / sqrt(0.4)));
			pixels[3 * (i + j*width) + 1] += f*(1 + cos(n*(dy*st + dx * ct)*M_2_PI));
			pixelsPrev[3 * (i + j*width) + 1] += f*(1 + cos(n*(dy*st + dx * ct)*M_2_PI + 0.3*sqrt(0.4) / sqrt(0.4)));
		}
	}
}

void MainGame::initFrame(GLManager& glManager) {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);


	std::vector<float> pixels(3 * width*height);
	
	
	//createWavePacket(pixels, pixelsPrev, width, height, 0.5, 0.5, 0.02, 20, 0);
	//    createWavePacket(pixels, pixelsPrev, width, height, 0.7, 0.5, 0.03, 500, 4*M_PI/3);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if ((i/50) % (width/(50*2)) == 0 || (j / 50) % (width / (50 * 2)) == 0) {
				pixels[3 * (i + j * width) + 0] = 0;
				pixels[3 * (i + j * width) + 1] = 0;
				pixels[3 * (i + j * width) + 2] = 0;
			}
			else {
				pixels[3 * (i + j * width) + 0] = 0;
				pixels[3 * (i + j * width) + 1] = 0;
				pixels[3 * (i + j * width) + 2] = 0;
			}
		}
	}

	for (int i = 0; i < 2; i++) {
		glGenTextures(1, &textures[i]);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGB, GL_FLOAT, &pixels[0]);
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void MainGame::HandleEvent(sf::Window& window, sf::Event & e)
{
	auto mpos = sf::Mouse::getPosition(window);
	switch (e.type) {
	case sf::Event::KeyPressed:
		switch (e.key.code) {
		case sf::Keyboard::C:

			captureStartLocation.x = std::max(0, mpos.x);
			captureStartLocation.y = std::max(0, mpos.y);

			break;
		case sf::Keyboard::P:
			loadRegion(exeDir() + "test.png", glm::ivec2(mpos.x, mpos.y));
			break;
		case sf::Keyboard::Space:
			paused = !paused;
			break;
		}
		break;
	case sf::Event::KeyReleased:
		switch (e.key.code) {
		case sf::Keyboard::C:

			glm::ivec2 npos;
			npos.x = std::max(0, mpos.x);
			npos.y = std::max(0, mpos.y);

			saveRegion(exeDir() + "test.png", captureStartLocation, npos);

			break;
		}
		break;
	}
}

void MainGame::saveRegion(const std::string& filename, glm::ivec2 first, glm::ivec2 last)
{

	glm::ivec2 s(std::min(first.x, last.x), std::min(height - 1 - first.y, height - 1 - last.y));
	glm::ivec2 e(std::max(first.x, last.x), std::max(height - 1 - first.y, height - 1 - last.y));

	glm::ivec2 diff = e - s;

	if (diff.x <= 0 || diff.y <= 0) return;

	std::vector<unsigned char> data(4*diff.x * diff.y);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glReadPixels(s.x,s.y, diff.x, diff.y, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

	sf::Image im;
	im.create(diff.x, diff.y, &data[0]);
	im.saveToFile(filename);
}

void MainGame::loadRegion(const std::string & filename, glm::ivec2 position)
{
	sf::Image im;
	im.loadFromFile(filename);
	auto size = im.getSize();

	

	std::vector<float> data(4 * size.x * size.y);

	auto startptr = im.getPixelsPtr();
	for (int i = 0; i < size.y; i++) {
		for (int j = 0; j < size.x; j++) {
			data[4 * (size.x * i + j) + 0] = ((float)startptr[4 * (size.x * i + j) + 0])/(256);
			data[4 * (size.x * i + j) + 1] = ((float)startptr[4 * (size.x * i + j) + 1])/(256);
			data[4 * (size.x * i + j) + 2] = ((float)startptr[4 * (size.x * i + j) + 2])/(256);
			data[4 * (size.x * i + j) + 3] = ((float)startptr[4 * (size.x * i + j) + 3])/(256);
		}
	}

	gl.Programs[0].Use();
	for (int i = 0; i < 2; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glTexSubImage2D(GL_TEXTURE_2D, 0, position.x, height - 1 - position.y, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, startptr);
	}
}

namespace std {
	template <> struct hash<std::pair<int, int>> {
		inline size_t operator()(const std::pair<int, int> &v) const {
			std::hash<int> int_hasher;
			return int_hasher(v.first) ^ int_hasher(v.second);
		}
	};
}

void MainGame::findStructures()
{
	//read frame;;

	using namespace std;
	using namespace glm;

	vector<vec4> data(width*height);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glReadPixels(0,0,width,height, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

	float meang;

	for (auto c : data) {
		meang += c.g;
	}
	meang /= data.size();

	unordered_map<pair<int, int>, pair<float, int>> map;

	for (int i = 0; i < width; i++) for (int j = 0; j < height; j++)
		if (data[j * width + i].g < 0.9 * meang)
			map.emplace(
				pair<int, int>(i, j),
				pair<float,int>(data[j * width + i].g, -1)
			);



}


