#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace grove::ml {

/// ONNX Runtime inference wrapper for lightweight ML models.
/// Loads a .onnx model and provides a simple predict interface.
/// Compile with GROVE_ENABLE_ONNX=ON to activate; otherwise stubs return empty.

class OnnxModel {
public:
    /// Load an ONNX model from file.
    /// @param model_path  Path to the .onnx file
    /// @throws std::runtime_error if ONNX Runtime is not available or model fails to load
    explicit OnnxModel(const std::string& model_path);
    ~OnnxModel();

    // Non-copyable, movable
    OnnxModel(const OnnxModel&) = delete;
    OnnxModel& operator=(const OnnxModel&) = delete;
    OnnxModel(OnnxModel&&) noexcept;
    OnnxModel& operator=(OnnxModel&&) noexcept;

    /// Run inference on a flat feature vector.
    /// @param features  Input features (must match model's expected input size)
    /// @return          Model output vector
    std::vector<double> predict(const std::vector<double>& features) const;

    /// Run inference on a batch of feature vectors.
    /// @param batch     Each inner vector is one sample's features
    /// @return          Batch of output vectors
    std::vector<std::vector<double>> predict_batch(
        const std::vector<std::vector<double>>& batch) const;

    /// Get model input names and shapes.
    std::vector<std::string> input_names() const;

    /// Get model output names and shapes.
    std::vector<std::string> output_names() const;

    /// Check if ONNX Runtime is available (compiled with GROVE_ENABLE_ONNX).
    static bool is_available();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace grove::ml
