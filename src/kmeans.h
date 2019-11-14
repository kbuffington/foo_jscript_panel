#pragma once

/*
k-means is a clustering algorithm designed to group data points into clusters of similar points,
and return the averaged "center" value of each cluster. The algorithm runs over the data set
multiple times, assigning points to the nearest center, and then re-calculating the center values
after each iteration, until the centers stabilize of max_iterations have been run.

Detailed information here: https://en.wikipedia.org/wiki/K-means_clustering

Here it is being used to group RGB colour values into clusters of similar colours for the purposes
of generating a colour scheme from an image. Each data point is a distinct RGB value that
represents a number of pixels with the same RGB value from the original image. Therefore while
every data-point is distinct, they do not all carry the same "weight" for the purposes of
determining the center points of each cluster.

In standard k-means, the starting center values are chosen at random. This provides better results
at the expense of potentially different values on subsequent runs with the same inputs. That was
not acceptable for generating colour values, so the starting center colour values are evenly spaced
across the data set.
*/

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

	int get_colour()
	{
		return 0xff000000 | static_cast<int>(central_values[0]) << 16 | static_cast<int>(central_values[1]) << 8 | static_cast<int>(central_values[2]);
	}

	int get_total_points()
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
	KMeans(int K, int total_points) : K(std::min(std::max(K, 14), total_points)), total_points(total_points) {}

	std::vector<Cluster> run(std::vector<KPoint>& points)
	{
		int index_point = 0;
		for (int i = 0; i < K; i++)
		{
			index_point = static_cast<int>(i * total_points / K);
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
					int cluster_total_points = cluster.get_total_points();
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
	int get_nearest_centre_id(KPoint point)
	{
		double min_dist;
		int cluster_centre_id = 0, it = 0;

		for (const Cluster& cluster : clusters)
		{
			double sum = 2 * pow(cluster.central_values[0] - point.m_values[0], 2.0);
			sum += 4 * pow(cluster.central_values[1] - point.m_values[1], 2.0);
			sum += 3 * pow(cluster.central_values[2] - point.m_values[2], 2.0);

			if (it == 0 || sum < min_dist)
			{
				min_dist = sum;
				cluster_centre_id = it;
			}
			it++;
		}

		return cluster_centre_id;
	}

	int K, total_points;
	std::vector<Cluster> clusters;
};
