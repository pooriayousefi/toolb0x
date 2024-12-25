export module random;

import <random>;
import <ranges>;
import <algorithm>;
import concepts;

// probability distribution simulators
export template<real_value_iterator It>
constexpr void simulate_beta_distribution_function(
	std::iter_value_t<It> alpha,
	std::iter_value_t<It> beta,
	std::mt19937_64& rng,
	It beg,
	It end
)
{
	// beta distributor
	auto distributor = [&]()
		{
			// beta distribution parameters assertion
			// parameters check (alpha > 0.0, beta > 0.0)
			if (alpha <= std::iter_value_t<It>{ 0 })
				throw std::runtime_error("alpha parameter (shape parameter) in beta distribution function must be greater than zero.");
			if (beta <= std::iter_value_t<It>{ 0 })
				throw std::runtime_error("beta parameter (shape parameter) in beta distribution function must be greater than zero.");
			std::gamma_distribution<double> parametrized_gamma_distribution_alpha(alpha, std::iter_value_t<It>{ 1 });
			std::gamma_distribution<double> parametrized_gamma_distribution_beta(beta, std::iter_value_t<It>{ 1 });
			return [parametrized_gamma_distribution_alpha, parametrized_gamma_distribution_beta](auto& random_number_generator) mutable
				{
					auto _{ parametrized_gamma_distribution_alpha(random_number_generator) };
					return _ / (_ + parametrized_gamma_distribution_beta(random_number_generator));
				};
		};
	auto rnd{ distributor() };
	std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
}

export template<real_value_iterator It>
constexpr void simulate_gamma_distribution_function(
	std::iter_value_t<It> alpha,
	std::iter_value_t<It> beta,
	std::mt19937_64& rng,
	It beg,
	It end
)
{
	// gamma distributor
	auto distributor = [&]()
		{
			// gamma distribution parameters assertion
			// parameters check (alpha > 0.0, beta > 0.0)
			if (alpha <= std::iter_value_t<It>{ 0 })
				throw std::runtime_error("alpha Parameter (shape Parameter) in gamma distribution function must be greater than zero.");
			if (beta <= std::iter_value_t<It>{ 0 })
				throw std::runtime_error("beta Parameter (scale Parameter) in gamma distribution function must be greater than zero.");
			std::gamma_distribution<double> parametrized_gamma_distribution(alpha, std::iter_value_t<It>{ 1 } / beta);
			return [parametrized_gamma_distribution](auto& random_number_generator) mutable
				{
					return parametrized_gamma_distribution(random_number_generator);
				};
		};
	auto rnd{ distributor() };
	std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
}

export template<real_value_iterator It>
constexpr void simulate_normal_distribution_function(
	std::iter_value_t<It> mu,
	std::iter_value_t<It> sigma,
	std::mt19937_64& rng,
	It beg,
	It end
)
{
	// normal distributor
	auto distributor = [&]()
		{
			// normal distribution parameters assertion
			// Parameter checking (0.0 <= mu <= 1.0)
			if (mu < std::iter_value_t<It>{ 0 } || mu > std::iter_value_t<It>{ 1 })
				throw std::runtime_error("mean value Parameter (mu) in normal distribution function"
					" must be between zero and one (it's a probability).");
			std::normal_distribution<double> parametrized_normal_distribution(mu, sigma);
			return [parametrized_normal_distribution](auto& random_number_generator) mutable
				{
					return parametrized_normal_distribution(random_number_generator);
				};
		};
	auto rnd{ distributor() };
	std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
}

export template<real_value_iterator It>
constexpr void simulate_lognormal_distribution_function(
	std::iter_value_t<It> EV,
	std::iter_value_t<It> EF,
	std::mt19937_64& rng,
	It beg,
	It end
)
{
	// log-normal distributor
	auto distributor = [&]()
		{
			// log-normal distribution parameters assertion
			// parameter checking (2nd Parameter - error factor - must be greater than 0.0)
			if (EF <= std::iter_value_t<It>{ 0 })
				throw std::runtime_error("error factor parameter in log-normal distribution function"
					" must be greater than zero.");
			auto sigma = (log(EF) / std::iter_value_t<It>{ 1.6449 });
			auto mu = (log(EV) - (sigma * sigma / std::iter_value_t<It>{ 2 }));
			std::lognormal_distribution<double> parametrized_lognormal_distribution(mu, sigma);
			return [parametrized_lognormal_distribution](auto& random_number_generator) mutable
				{
					return parametrized_lognormal_distribution(random_number_generator);
				};
		};
	auto rnd{ distributor() };
	std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
}

export template<real_value_iterator It>
constexpr void simulate_uniform_distribution_function(
	std::iter_value_t<It> minval,
	std::iter_value_t<It> maxval,
	std::mt19937_64& rng,
	It beg,
	It end
)
{
	// uniform distributor
	auto distributor = [&]()
		{
			// uniform distribution parameters assertion
			// Parameter checking (both parameters must be between zero and one)
			if (minval < std::iter_value_t<It>{ 0 } || minval > std::iter_value_t<It>{ 1 })
				throw std::runtime_error("minimum value Parameter in uniform distribution function"
					" must be between zero and one (it's a probability).");
			if (maxval < std::iter_value_t<It>{ 0 } || maxval > std::iter_value_t<It>{ 1 })
				throw std::runtime_error("maximum value Parameter uniform distribution function"
					" must be between zero and one (it's a probability).");
			std::uniform_real_distribution<double> parametrized_uniform_distribution(minval, maxval);
			return [parametrized_uniform_distribution](auto& random_number_generator) mutable
				{
					return parametrized_uniform_distribution(random_number_generator);
				};
		};
	auto rnd{ distributor() };
	std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
}

export template<real_value_iterator It>
constexpr void simulate_loguniform_distribution_function(
	std::iter_value_t<It> minval,
	std::iter_value_t<It> maxval,
	std::mt19937_64& rng,
	It beg,
	It end
)
{
	// log-uniform distributor
	auto distributor = [&]()
		{
			// log-uniform distribution parameters assertion
			// parameter checking (both parameters must be greater than zero, 
				// because, there is no value for negative logarithms)
			if (minval <= std::iter_value_t<It>{ 0 })
				throw std::runtime_error("minimum value parameter in log-uniform distribution function"
					" must be greater than zero (there is no value for zero-negative logarithms)");
			if (maxval <= std::iter_value_t<It>{ 0 })
				throw std::runtime_error("maximum value parameter in log-uniform distribution function"
					" must be greater than zero (there is no value for zero-negative logarithms)");
			std::uniform_real_distribution<double> parametrized_loguniform_distribution(minval, maxval);
			return [parametrized_loguniform_distribution](auto& random_number_generator) mutable
				{
					return exp(parametrized_loguniform_distribution(random_number_generator));
				};
		};
	auto rnd{ distributor() };
	std::ranges::generate(beg, end, [&rng, &rnd]() { return rnd(rng); });
}
