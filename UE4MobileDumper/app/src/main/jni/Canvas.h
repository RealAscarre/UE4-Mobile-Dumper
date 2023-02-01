//
// Created by Ascarre on 01-02-2023.
//

#ifndef PRIVATE_DUMPER_CANVAS_H
#define PRIVATE_DUMPER_CANVAS_H

/******************** Vector2 Structure ********************/
struct Vector2 {
    float x;
    float y;

    Vector2() {
        this->x = 0;
        this->y = 0;
    }

    Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    static Vector2 Zero() {
        return Vector2(0.0f, 0.0f);
    }

    bool operator!=(const Vector2 &src) const {
        return (src.x != x) || (src.y != y);
    }

    Vector2 &operator+=(const Vector2 &v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2 &operator-=(const Vector2 &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

};

/******************** Color Class ********************/
class Color {
public:
    float r;
    float g;
    float b;
    float a;

    Color() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
        this->a = 0;
    }

    Color(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    Color(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 255;
    }
};

/******************** Canvas Class ********************/
class Canvas {
private:
    JNIEnv *_env;
    jobject _cvsView;
    jobject _cvs;

public:
    Canvas() {
        _env = nullptr;
        _cvsView = nullptr;
        _cvs = nullptr;
    }

    Canvas(JNIEnv *env, jobject cvsView, jobject cvs) {
        this->_env = env;
        this->_cvsView = cvsView;
        this->_cvs = cvs;
    }

    bool isValid() const {
        return (_env != nullptr && _cvsView != nullptr && _cvs != nullptr);
    }

    int GetWidth() const {
        if (isValid()) {
            jclass canvas = _env->GetObjectClass(_cvs);
            jmethodID Width = _env->GetMethodID(canvas, "getWidth", "()I");
            return _env->CallIntMethod(_cvs, Width);
        }
        return 0;
    }

    int GetHeight() const {
        if (isValid()) {
            jclass canvas = _env->GetObjectClass(_cvs);
            jmethodID height = _env->GetMethodID(canvas, "getHeight", "()I");
            return _env->CallIntMethod(_cvs, height);
        }
        return 0;
    }

    void DrawText(Color color, const char *txt, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawText","(Landroid/graphics/Canvas;IIIILjava/lang/String;FFF)V");
            _env->CallVoidMethod(_cvsView, drawtext, _cvs, (int) color.a, (int) color.r, (int) color.g, (int) color.b, _env->NewStringUTF(txt), pos.x, pos.y, size);
        }
    }
};

#endif //PRIVATE_DUMPER_CANVAS_H
