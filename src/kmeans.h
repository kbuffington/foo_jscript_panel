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

class KPoint
{
public:
	KPoint(int id_point, const std::vector<unsigned int>& values, int pixel_count) : id_point(id_point), values(values), pixel_count(pixel_count)
	{
		id_cluster = -1;
	}

	int id_cluster;
	int id_point;
	int pixel_count;
	std::vector<unsigned int> values;
};

class Cluster
{
public:
	Cluster(KPoint point)
	{
		std::transform(point.values.begin(), point.values.end(), std::back_inserter(central_values), [](const unsigned int value)
		{
			return static_cast<double>(value);
		});

		points.emplace_back(point);
	}

	bool removePoint(int id_point) {
		int total_points = points.size();

		for (int i = 0; i < total_points; i++)
		{
			if (points[i].id_point == id_point)
			{
				points.erase(points.begin() + i);
				return true;
			}
		}
		return false;
	}

	int getColour()
	{
		return 0xff000000 | static_cast<int>(central_values[0]) << 16 | static_cast<int>(central_values[1]) << 8 | static_cast<int>(central_values[2]);
	}

	int getTotalPoints() {
		return std::accumulate(points.begin(), points.end(), 0, [](int t, const KPoint& point)
		{
			return t + point.pixel_count;
		});
	}

	std::vector<double> central_values;
	std::vector<KPoint> points;
};

class KMeans
{
public:
	KMeans(int K, int total_points) : K(std::min(std::max(K, 14), total_points)), total_points(total_points) {}

	std::vector<Cluster> run(std::vector<KPoint>& points) {
		int index_point = 0;
		for (int i = 0; i < K; i++) {
			index_point = static_cast<int>(i * total_points / K);
			points[index_point].id_cluster = i;
			Cluster cluster(points[index_point]);
			clusters.emplace_back(cluster);
		}

		int iter = 1;

		while (true) {
			bool done = true;

			for (int i = 0; i < total_points; i++) {
				int id_old_cluster = points[i].id_cluster;
				int id_nearest_center = getIDNearestCenter(points[i]);

				if (id_old_cluster != id_nearest_center) {
					if (id_old_cluster != -1) {
						clusters[id_old_cluster].removePoint(points[i].id_point);
					}

					points[i].id_cluster = id_nearest_center;
					clusters[id_nearest_center].points.emplace_back(points[i]);
					done = false;
				}
			}

			for (int i = 0; i < K; i++) {
				for (int j = 0; j < kNumColourComponents; j++) {
					int total_points_cluster = clusters[i].getTotalPoints();
					double sum = 0.0;

					if (total_points_cluster > 0) {
						for (const KPoint& point : clusters[i].points)
						{
							sum += static_cast<double>(point.values[j]) * static_cast<double>(point.pixel_count);
						}
						clusters[i].central_values[j] = sum / total_points_cluster;
					}
				}
			}

			if (done || iter >= kMaxIterations) {
				break;
			}

			iter++;
		}

		return clusters;
	}

private:
	int getIDNearestCenter(KPoint point) {
		double sum, min_dist;
		int id_cluster_center = 0;

		for (int i = 0; i < K; i++) {
			sum = 0.0;
			sum += 2 * pow(clusters[i].central_values[0] - point.values[0], 2.0);
			sum += 4 * pow(clusters[i].central_values[1] - point.values[1], 2.0);
			sum += 3 * pow(clusters[i].central_values[2] - point.values[2], 2.0);

			if (i == 0 || sum < min_dist) {
				min_dist = sum;
				id_cluster_center = i;
			}
		}

		return id_cluster_center;
	}

	int K, total_points;
	std::vector<Cluster> clusters;
};
