#include <vector>
#include "tile.h"
#include "font.h"
#include "input.h"
#include "material.h"
#include <memory>

struct Camera;

class Engine;
class AInputEvent;

struct ScreenLayout;

struct GameLoop : InputListener {
private:
	friend struct Engine;
	Engine* engine;

	Font font;

	Material material;

	void init();

	void load();
	void unload();

	void destroy();
	void tick(double);

	void OnPointerDown(const Pointer &pointer) override;

	void OnPointerMove(const Pointer &pointer) override;

	void OnPointerUp(const Pointer &pointer) override;

public:
	void quit();
};
