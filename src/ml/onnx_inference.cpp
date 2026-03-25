#include "grove/ml/onnx_inference.h"

#include <stdexcept>

namespace grove::ml {

#ifdef GROVE_HAS_ONNX

// ─── Real ONNX Runtime Implementation ──────────────────────────────
// TODO: Implement when ONNX Runtime is linked
// #include <onnxruntime_cxx_api.h>

struct OnnxModel::Impl {
    // Ort::Session session;
    // Ort::Env env;
};

#else

// ─── Stub Implementation (ONNX Runtime not available) ───────────────

struct OnnxModel::Impl {};

#endif

OnnxModel::OnnxModel(const std::string& model_path) : impl_(std::make_unique<Impl>()) {
    if (!is_available()) {
        throw std::runtime_error(
            "ONNX Runtime is not available. Rebuild grove with -DGROVE_ENABLE_ONNX=ON");
    }
}

OnnxModel::~OnnxModel() = default;
OnnxModel::OnnxModel(OnnxModel&&) noexcept = default;
OnnxModel& OnnxModel::operator=(OnnxModel&&) noexcept = default;

std::vector<double> OnnxModel::predict(const std::vector<double>& features) const {
    throw std::runtime_error("ONNX Runtime not available");
}

std::vector<std::vector<double>> OnnxModel::predict_batch(
    const std::vector<std::vector<double>>& batch) const {
    throw std::runtime_error("ONNX Runtime not available");
}

std::vector<std::string> OnnxModel::input_names() const { return {}; }
std::vector<std::string> OnnxModel::output_names() const { return {}; }

bool OnnxModel::is_available() {
#ifdef GROVE_HAS_ONNX
    return true;
#else
    return false;
#endif
}

} // namespace grove::ml
