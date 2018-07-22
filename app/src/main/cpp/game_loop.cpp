#include "game_loop.h"
#include "engine.h"
#include "behaviour.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "camera.h"

struct TubeManager : public Behaviour {
    std::vector<SpriteRenderer*> tubes;
    Transform2D* birdTransform;

    void update(double d) override {
        while (birdTransform->position.x > (tubes[0]->transform()->position.x + 38)) {
            auto tube = tubes.front();

            auto tt = tube->transform();

            tubes.erase(tubes.begin());

            tube->clip = 1 - tubes.back()->clip;
            tt->size.y = 50.f + ((rand() / (float) RAND_MAX) - 0.5f) * 25.f;
            tt->position.x = tubes.back()->transform()->position.x + 28;
            tt->position.y = (tube->clip % 2) ? (tt->size.y - 160) : (160 - tt->size.y);
            tubes.push_back(tube);
        }
    }

    void reset() {
        if (tubes.empty()) {
            for (int i = 0; i < 11; i++) {
                tubes.push_back((new GameObject)->AddComponent<SpriteRenderer>());
            }
        }

        for (int i = 0; i < 11; i++) {
            auto tube = tubes[i];

            float _h = 50.f + ((rand() / (float) RAND_MAX) * 2.f - 1.f) * 25.f;
            tube->transform()->position = {Camera::main->aspect * 138 + i * 28.f,  (i % 2) ? (160 - _h) : (_h - 160)};
            tube->transform()->size = {28, _h};
            tube->clip = 1 - (i % 2);
        }
    }
};

struct Bird : public Behaviour {
    SpriteRenderer* renderer;

    TubeManager* tubeManager;

    vec2 velocity = {0, 0};
    vec2 acceleration = {0, 100};

    double time = 0;

    void update(double delta) override {
        if (!alive()) {
            reset();
            tubeManager->reset();
        }

        //todo: collision manager
        auto bt = transform();
        auto br = Rect(bt->position.x - bt->size.x * 0.5f, bt->position.y - bt->size.y * 0.5f, bt->size.x, bt->size.y);

        for (auto tube : tubeManager->tubes) {
            auto tt = tube->transform();

            Rect tr = Rect(tt->position.x - tt->size.x * 0.5f, tt->position.y, tt->size.x, 160);

            if (tr.contains(br.x, br.y) ||
                tr.contains(br.x + br.w, br.y) ||
                tr.contains(br.x, br.y + br.h) ||
                tr.contains(br.x + br.w, br.y + br.h)
            ) {

                reset();
                tubeManager->reset();
                return;
            }
        }

        auto tr = transform();

        velocity += acceleration * delta;

        if (velocity.y > 100) {
            velocity.y = 100;
        }

        tr->position = tr->position - velocity * delta;

        if (tr->position.y > 138) {
            tr->position.y = 138;
        }

        if (velocity.y < 0) {
            tr->rotation -= 600 * delta;

            if (tr->rotation < -20) {
                tr->rotation = -20;
            }

            renderer->clip = int(time += (delta * 4.f)) % 3;
        } else if (falling()) {
            tr->rotation += 300 * delta;
            if (tr->rotation > 90) {
                tr->rotation = 90;
            }
            renderer->clip = 0;
        }

        Camera::main->transform.position.x = -transform()->position.x + 33;
    }

    bool falling() {
        return velocity.y > 18;
    }

    bool alive() {
        return transform()->position.y > 0;
    }

    inline void reset() {
        transform()->position = {33, 138 * 0.5f - 6};
        transform()->rotation = 0;
        transform()->size = {17, 12};
        velocity = {-20, 0};
        time = 0;

        Camera::main->transform.position.x = -transform()->position.x + 33;
    }
};

Bird* bird = nullptr;
TubeManager* tubeManager = nullptr;

void GameLoop::init() {
    InputManager::AddListener(this);

	ResourceManager::Init(engine->asset_manager);

    Camera::main = new Camera;
    Camera::main->viewport = {0, 0, 1, 1};
    Camera::main->color = Color::Cyan;

    auto tubeManagerGameObject = new GameObject;
    tubeManager = tubeManagerGameObject->AddComponent<TubeManager>();

    auto birdGameObject = new GameObject;
    bird = birdGameObject->AddComponent<Bird>();
    bird->tubeManager = tubeManager;
    bird->renderer = birdGameObject->AddComponent<SpriteRenderer>();

    tubeManager->birdTransform = bird->transform();
}

void GameLoop::destroy() {
    InputManager::RemoveListener(this);

	delete this;
}

void GameLoop::load() {
    Camera::main->aspect = static_cast<float>(Screen::width()) / static_cast<float>(Screen::height());

    material = Material("vertex.glsl", "fragment.glsl");

    font = Font::Create("font/font.png", "font/font.cfg", "font/font.txt");

    auto tubeSprite = Sprite::Create("tubes.png", {0, 0, 28, 160}, {0.5f, 0.0f});

    bird->reset();
    tubeManager->reset();

    for (auto tube : tubeManager->tubes) {
        tube->sprite = tubeSprite;
    }

    bird->renderer->sprite = Sprite::Create("bird.png", {0, 0, 17, 12});

    glUseProgram(material.shader);

    auto v = Camera::main->viewport;

    int32_t w = Screen::width();
    int32_t h = Screen::height();

    glViewport(static_cast<GLint>(w * v.x), static_cast<GLint>(h * v.y), static_cast<GLsizei>(w * v.w), static_cast<GLsizei>(h * v.z));
}

void GameLoop::unload() {
    material.release();
}

void GameLoop::tick(double delta) {
    auto& color = Camera::main->color;

    for (auto go : Scene::gameObjects) {
        if (!go->active) continue;

        auto behaviours = go->GetComponents<Behaviour>();

        for (auto behaviour : behaviours) {
            if (!behaviour->enabled) continue;

            behaviour->update(delta);
        }
    }

    glClearColor(color.r / 255.f, color.g / 255.0f, color.b / 255.0f, color.a / 255.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(material.WORLD_MATRIX, 1, GL_FALSE, &(Camera::main->matrix())[0][0]);

    for (auto go : Scene::gameObjects) {
        if (!go->active) continue;

        auto renderers = go->GetComponents<Renderer>();

        for (auto renderer : renderers) {
            if (!renderer->enabled) continue;

            renderer->render(material);
        }
    }
}

void GameLoop::quit() {
    auto jvm = engine->java_vm;

    JNIEnv* env;

    jvm->AttachCurrentThread(&env, NULL);
    env->CallVoidMethod(engine->activity, engine->quitMethodID);
    jvm->DetachCurrentThread();
}

void GameLoop::OnPointerDown(const Pointer &pointer) {
    bird->velocity.y = -80;
}

void GameLoop::OnPointerMove(const Pointer &pointer) {

}

void GameLoop::OnPointerUp(const Pointer &pointer) {

}
