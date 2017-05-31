#include "util.h"

void Util::print_vec3(glm::vec3 v) {
    std::cerr << v.x << ", " << v.y << ", " << v.z << std::endl;
}

void Util::print_mat4(glm::mat4 m) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cerr << m[j][i] << "\t";
        }
        std::cerr << std::endl;
    }
}

unsigned char *Util::loadPPM(const char *filename, int &width, int &height) {
    const int BUFSIZE = 128;
    FILE *fp;
    unsigned int read;
    unsigned char *rawData;
    char buf[3][BUFSIZE];
    char *retval_fgets;
    size_t retval_sscanf;

    if ((fp = fopen(filename, "rb")) == NULL) {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }

    // Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);

    // Read width and height:
    do {
        retval_fgets = fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width = atoi(buf[1]);
    height = atoi(buf[2]);

    // Read maxval:
    do {
        retval_fgets = fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');

    // Read image data:
    rawData = new unsigned char[width * height * 3];
    read = (unsigned int) fread(rawData, width * height * 3, 1, fp);
    fclose(fp);
    if (read != 1) {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }
    return rawData;
}

glm::mat4 Util::calc_bezier_mat(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    // column-wise
    float points[16] = {
            p0.x, p0.y, p0.z, 0.f,
            p1.x, p1.y, p1.z, 0.f,
            p2.x, p2.y, p2.z, 0.f,
            p3.x, p3.y, p3.z, 0.f
    };
    glm::mat4 G;
    memcpy(glm::value_ptr(G), points, sizeof(points));

    float Bcoeffs[16] = {
            -1.f, 3.f, -3.f, 1.f,
            3.f, -6.f, 3.f, 0.f,
            -3.f, 3.f, 0.f, 0.f,
            1.f, 0.f, 0.f, 0.f
    };
    glm::mat4 B;
    memcpy(glm::value_ptr(B), Bcoeffs, sizeof(Bcoeffs));

    glm::mat4 ret = G * B;
    ret[3][3] = 1.f;
    return ret;
}

glm::vec3 Util::trackball_position(double x_pos, double y_pos, int width, int height) {
    glm::vec3 ret(0.0f);
    float d;
    ret.x = (float) (2 * x_pos - width) / width;
    ret.y = (float) (height - 2.0 * y_pos) / height;
    ret.z = 0.0;
    d = glm::length(ret);
    d = (d < 1.f) ? d : 1.f;
    ret.z = (float) glm::sqrt(1.001 - d * d);
    ret = glm::normalize(ret);
    return ret;
}

void Util::seed(unsigned int s) {
    if (s == 0)
        srand((unsigned int) time(NULL));
    else
        srand(s);
}

float Util::random(float min, float max) {
    float random = ((float) rand()) / (float) (RAND_MAX + 1);
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

bool Util::within_rect(glm::vec2 pos, glm::vec2 bottom_left, glm::vec2 top_right) {
    if (pos.x > bottom_left.x && pos.x < top_right.x && pos.y > top_right.y && pos.y < bottom_left.y) {
        return true;
    }
    return false;
}

std::pair<std::string, std::string> Util::wordbreak_text(std::string text, unsigned long chars_per_line) {
    // If line is short enough, just chomp all of it
    if (text.length() <= chars_per_line) {
        return std::make_pair(text, "");
    }

    // Find a space to cut at
    size_t cut_at = text.find_last_of(" ", chars_per_line);
    std::string line;

    // If we couldn't find a suitable breakpoint, we need to hyphenate
    if (cut_at == std::string::npos) {
        return std::make_pair(text.substr(0, chars_per_line - 1) + "-", text.substr(chars_per_line - 1));
    }

    // Otherwise, cut at the space we found
    line = text.substr(0, cut_at);
    text = text.substr(cut_at);

    // Cut out the extra space at the next start of line
    size_t first_nonspace = text.find_first_not_of(" ");
    if (first_nonspace != std::string::npos)
        text = text.substr(first_nonspace);

    return std::make_pair(line, text);
};
