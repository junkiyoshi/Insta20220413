#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofEnableDepthTest();
	ofSetLineWidth(3);

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 60 < 35) {

		this->noise_param += ofMap(ofGetFrameNum() % 60, 0, 35, 0.2, 0);
	}

	this->face.clear();
	this->line.clear();

	float phi_deg_step = 0.5;
	float theta_deg_step = 0.5;
	float theta_start = 70;
	float theta_end = 110;
	float threshold = 0.45;

	float radius = 150;

	for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

		for (float theta_deg = theta_start; theta_deg <= theta_end; theta_deg += theta_deg_step) {

			auto noise_value = ofNoise(
				radius * cos(phi_deg * DEG_TO_RAD) * 0.02,
				radius * sin(phi_deg * DEG_TO_RAD) * 0.02,
				radius * cos(theta_deg * DEG_TO_RAD) * 0.02,
				this->noise_param);

			if (noise_value < threshold) { continue; }

			auto noise_value_1 = ofNoise(
				radius * cos(phi_deg * DEG_TO_RAD) * 0.02,
				radius * sin(phi_deg * DEG_TO_RAD) * 0.02,
				radius * cos((theta_deg - theta_deg_step) * DEG_TO_RAD) * 0.02,
				this->noise_param);
			auto noise_value_2 = ofNoise(
				radius * cos((phi_deg + phi_deg_step) * DEG_TO_RAD) * 0.02,
				radius * sin((phi_deg + phi_deg_step) * DEG_TO_RAD) * 0.02,
				radius * cos(theta_deg * DEG_TO_RAD) * 0.02,
				this->noise_param);
			auto noise_value_3 = ofNoise(
				radius * cos((phi_deg - phi_deg_step) * DEG_TO_RAD) * 0.02,
				radius * sin((phi_deg - phi_deg_step) * DEG_TO_RAD) * 0.02,
				radius * cos(theta_deg * DEG_TO_RAD) * 0.02,
				this->noise_param);
			auto noise_value_4 = ofNoise(
				radius * cos(phi_deg * DEG_TO_RAD) * 0.02,
				radius * sin(phi_deg * DEG_TO_RAD) * 0.02,
				radius * cos((theta_deg + theta_deg_step) * DEG_TO_RAD) * 0.02,
				this->noise_param);

			auto index = this->face.getNumVertices();
			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(
				radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
			vertices.push_back(glm::vec3(
				radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
			vertices.push_back(glm::vec3(
				radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));
			vertices.push_back(glm::vec3(
				radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));

			this->face.addVertices(vertices);

			this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
			this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

			if (noise_value_1 < threshold || theta_deg == theta_start) {

				this->line.addVertex(vertices[0]); this->line.addVertex(vertices[1]);
				this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
			}

			if (noise_value_2 < threshold) {

				this->line.addVertex(vertices[0]); this->line.addVertex(vertices[2]);
				this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
			}

			if (noise_value_3 < threshold) {

				this->line.addVertex(vertices[1]); this->line.addVertex(vertices[3]);
				this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
			}

			if (noise_value_4 < threshold || theta_deg == theta_end) {

				this->line.addVertex(vertices[2]); this->line.addVertex(vertices[3]);
				this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(270);
	ofRotateZ(ofGetFrameNum());

	ofSetColor(255);
	this->face.draw();
	ofSetColor(0);
	this->line.draw();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}