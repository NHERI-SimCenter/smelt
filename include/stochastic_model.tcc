/**
 * @contructor Constructor with multiple arguments
 */
template <typename... Targs>
StochasticModel::StochasticModel(Targs... args) : model_name_{"Abstract"} {}
