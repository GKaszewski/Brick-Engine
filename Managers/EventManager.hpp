#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

enum class EventType {
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1, Mouse, Joystick
};

struct EventInfo {
	EventInfo() { code = 0; }
	EventInfo(int event) { code = event; }
	union {
		int code;
	};
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct EventDetails {
	EventDetails(const std::string& bindName);

	std::string name;
	sf::Vector2i size;
	sf::Uint32 textEntered;
	sf::Vector2i mouse;
	int mouseWheelDelta;
	int keyCode;

	void clear();
};

struct Binding {
	Binding(const std::string& name);

	void bindEvent(EventType type, EventInfo info = EventInfo());

	Events events;
	std::string name;
	int c;

	EventDetails details;
};

using Bindings = std::unordered_map<std::string, Binding*>;
using Callbacks = std::unordered_map<std::string, std::function<void(EventDetails*)>>;

class EventManager {
public:
	EventManager();
	~EventManager();
	
	bool addBinding(Binding* binding);
	bool removeBinding(const std::string& name);

	inline void setFocus(const bool& focus) { hasFocus = focus; }
	template<typename T>
	bool addCallback(const std::string& name, void(T::* func)(EventDetails*), T* instance) {
		auto temp = std::bind(func, instance, std::placeholders::_1);
		return callbacks.emplace(name, temp).second;
	}

	void removeCallback(const std::string& name) { callbacks.erase(name); }

	void handleEvent(sf::Event& event);
	void update();
	
	sf::Vector2i getMousePos(sf::RenderWindow* window = nullptr);
private:
	void loadBindings();

	Bindings bindings;
	Callbacks callbacks;
	bool hasFocus;
};

