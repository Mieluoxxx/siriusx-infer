#include "op/add.h"

#include "base/base.h"
#include "kernels/interface.h"

namespace op {
VecAddLayer::VecAddLayer(base::DeviceType device_type)
    : Layer(device_type, LayerType::LayerAdd, "Add") {
    reset_input_size(2);
    reset_output_size(1);
}

base::Status VecAddLayer::check() const {
    tensor::Tensor in1 = this->get_input(0);
    tensor::Tensor in2 = this->get_input(1);
    int32_t size = in1.size();
    base::Status status;
    status = check_tensor_with_dim(in1, device_type_, data_type_, size);
    if (!status) {
        LOG(ERROR) << "The input tensor 1 error in the add layer.";
        return status;
    }

    status = check_tensor_with_dim(in2, device_type_, data_type_, size);
    if (!status) {
        LOG(ERROR) << "The input tensor 2 error in the add layer.";
        return status;
    }

    status =
        check_tensor_with_dim(get_output(0), device_type_, data_type_, size);
    if (!status) {
        LOG(ERROR) << "The output tensor error in the add layer.";
        return status;
    }
    return base::error::Success();
}

base::Status VecAddLayer::forward() {
    auto status = this->check();
    if (!status) {
        return status;
    }
    auto in1 = this->get_input(0);
    auto in2 = this->get_input(1);
    auto out = this->get_output(0);
    if (device_type_ == base::DeviceType::CUDA) {
        LOG(ERROR) << "CUDA is not supported yet.";
    }
    kernel::get_add_kernel(device_type_)(in1, in2, out,
                                         nullptr);  // TODO: add cuda stream
    return base::error::Success();
}
}  // namespace op