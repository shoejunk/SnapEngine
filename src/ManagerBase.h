#pragma once

#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <stdexcept>

class ManagerBase {
protected:
	// Container for storing JSON objects.
	std::vector<nlohmann::json> jsonObjects;

public:
	// Virtual destructor to ensure proper cleanup in derived classes.
	virtual ~ManagerBase() {}

	// Function to add a JSON object to the manager.
	void addJsonObject(const nlohmann::json& jsonObject) {
		jsonObjects.push_back(jsonObject);
	}

	// Function to retrieve all stored JSON objects.
	const std::vector<nlohmann::json>& getJsonObjects() const {
		return jsonObjects;
	}

	// Pure virtual function to be implemented by derived classes to create objects.
	virtual void createObjects() = 0;
};

// Example derived class to demonstrate functionality.
class ExampleManager : public ManagerBase {
public:
	void createObjects() override {
		for (const auto& jsonObject : jsonObjects) {
			// Implement logic for creating objects using the jsonObject data.
			if (jsonObject.contains("name")) {
				std::string name = jsonObject["name"];
				// Example: Print the name field from the JSON object.
				std::cout << "Creating object with name: " << name << std::endl;
			}
			else {
				throw std::runtime_error("JSON object does not contain required field: name");
			}
		}
	}
};
