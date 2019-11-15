#pragma once

static constexpr int kNumColourComponents = 3;
static constexpr int kMaxIterations = 12;

struct KPoint
{
	KPoint(int p_point_id, const std::vector<unsigned int>& p_values, int p_pixel_count) : m_point_id(p_point_id), m_values(p_values), m_pixel_count(p_pixel_count), m_cluster_id(-1) {}

	int m_cluster_id, m_point_id, m_pixel_count;
	std::vector<unsigned int> m_values;
};

class Cluster
{
public:
	Cluster(KPoint point)
	{
		std::transform(point.m_values.begin(), point.m_values.end(), std::back_inserter(central_values), [](const unsigned int value)
			{
				return static_cast<double>(value);
			});
		points.emplace_back(point);
	}

	double calc_dist(const KPoint& point) const
	{
		double sum = 2 * pow(central_values[0] - point.m_values[0], 2.0);
		sum += 4 * pow(central_values[1] - point.m_values[1], 2.0);
		sum += 3 * pow(central_values[2] - point.m_values[2], 2.0);
		return sum;
	}

	int get_colour()
	{
		return 0xff000000 | static_cast<int>(central_values[0]) << 16 | static_cast<int>(central_values[1]) << 8 | static_cast<int>(central_values[2]);
	}

	int get_total_points() const
	{
		return std::accumulate(points.begin(), points.end(), 0, [](int t, const KPoint& point)
			{
				return t + point.m_pixel_count;
			});
	}

	void remove_point(int point_id)
	{
		auto it = std::remove_if(points.begin(), points.end(), [point_id](const KPoint& point)
			{
				return point.m_point_id == point_id;
			});
		points.erase(it, points.end());
	}

	std::vector<double> central_values;
	std::vector<KPoint> points;
};

class KMeans
{
public:
	KMeans(int p_count, int p_total_points) : count(std::min(std::max(p_count, 14), p_total_points)), total_points(p_total_points) {}

	std::vector<Cluster> run(std::vector<KPoint>& points)
	{
		for (int i = 0; i < count; i++)
		{
			const int index_point = static_cast<int>(i * total_points / count);
			points[index_point].m_cluster_id = i;
			Cluster cluster(points[index_point]);
			clusters.emplace_back(cluster);
		}

		int iter = 1;

		while (true)
		{
			bool done = true;

			for (int i = 0; i < total_points; ++i)
			{
				int old_cluster_id = points[i].m_cluster_id;
				int nearest_centre_id = get_nearest_centre_id(points[i]);

				if (old_cluster_id != nearest_centre_id)
				{
					if (old_cluster_id != -1)
					{
						clusters[old_cluster_id].remove_point(points[i].m_point_id);
					}

					points[i].m_cluster_id = nearest_centre_id;
					clusters[nearest_centre_id].points.emplace_back(points[i]);
					done = false;
				}
			}

			for (Cluster& cluster : clusters)
			{
				for (int j = 0; j < kNumColourComponents; ++j)
				{
					const int cluster_total_points = cluster.get_total_points();
					if (cluster_total_points == 0) continue;

					double sum = std::accumulate(cluster.points.begin(), cluster.points.end(), 0.0, [j](double t, const KPoint& point)
						{
							return t + (static_cast<double>(point.m_values[j]) * static_cast<double>(point.m_pixel_count));
						});

					cluster.central_values[j] = sum / cluster_total_points;
				}
			}

			if (done || iter >= kMaxIterations)
			{
				break;
			}

			iter++;
		}

		return clusters;
	}

private:
	int get_nearest_centre_id(const KPoint& point)
	{
		double min_dist;
		int cluster_centre_id = 0, it = 0;

		for (const Cluster& cluster : clusters)
		{
			const double dist = cluster.calc_dist(point);

			if (it == 0 || dist < min_dist)
			{
				min_dist = dist;
				cluster_centre_id = it;
			}
			it++;
		}

		return cluster_centre_id;
	}

	int count, total_points;
	std::vector<Cluster> clusters;
};
