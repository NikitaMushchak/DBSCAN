#include <stdio.h>
#include <iostream>
#include "dbscan.h"
#include "ai.hh"
#include "datareader.hh"
#include "cluster.hh"
#include "dispersion.hh"
#include "dbscan.h"


#define MINIMUM_POINTS 5     // minimum number of cluster
#define EPSILON (1.1*1.1)  // distance for clustering, metre^2

void readBenchmarkData(vector<Point>& points)
{
    // load point cloud
    FILE *stream;

    //ai::printMarker();

    stream = fopen ("ini.xyz","r");

    //ai::printMarker();

    if (stream==NULL)
      {
        std::cout << "Hello" << std::endl;
      }

    unsigned int minpts, num_points, cluster, i = 0;
    double epsilon;
    fscanf(stream, "%ud", &num_points);



    std::cout << num_points << std::endl;

    Point *p = (Point *)calloc(num_points, sizeof(Point));

    ai::printMarker();

    while (i < num_points)
    {
            char C;
          fscanf(stream, "%c %f %f %f\n",&C, &(p[i].x), &(p[i].y), &(p[i].z));
          p[i].clusterID = UNCLASSIFIED;
          points.push_back(p[i]);
           //std::cout<<"data: "<<p[i].x<<" "<<p[i].y<<" "<<p[i].z<<std::endl;
          ++i;
    }

    free(p);
    fclose(stream);
}


void readClusterData(vector<Point>& points)
{
    // load point cloud
    std::vector<std::vector<double> > xyz;
    //std::cout<<"s";
    //ai::printMarker();
    //FILE *stream;
    //stream = fopen ("ini.dat","ra");
    //std::cout<<"data reading";
    unsigned int minpts, num_points, cluster, i = 0;
    double epsilon;
    //fscanf(stream, "%u\n", &num_points);

    ai::printMarker();
    ai::loadXYZ(
         "ini.xyz",
        xyz);
        //ai::printMatrix(xyz);
    num_points = xyz.size();

    //ai::printMarker();

    std::cout<<"number points = "<<num_points<<std::endl;

    Point *p = (Point *)calloc(num_points, sizeof(Point));

    while (i < num_points)
    {
            //char C;
          //fscanf(stream, "%s %f %f %f\n",C, &(p[i].x), &(p[i].y), &(p[i].z));
             p[i].x = xyz[i][0];
             p[i].y = xyz[i][1];
             p[i].z = xyz[i][2];
          p[i].clusterID = UNCLASSIFIED;
          points.push_back(p[i]);
          ++i;
          // std::cout<<"data: "<<p[i].x<<" "<<p[i].y<<" "<<p[i].z<<std::endl;
    }

    free(p);
    //fclose(stream);
}
void readDumpData(vector<Point>& points , std::string fname){

    std::vector<TParticle> particles;

	int swap_bytes = 0;
	TDump_config_info info;

	 load_part_dump(fname,
                   particles,
                    info,
                    swap_bytes);

    std::size_t number = particles.size();

    std::cout<<"Number of particles = "<<number<<std::endl;

    Point *p = (Point *)calloc(number, sizeof(Point));

    std::vector <TParticle> ::iterator po;
    size_t i=0;
			for ( po = particles.begin(); po < particles.end(); po++) {
                p[i].x = po->r.x;
                p[i].y = po->r.y;
                p[i].z = po->r.z;

                p[i].clusterID = UNCLASSIFIED;

                p[i].vx = po->v.x;
                p[i].vy = po->v.y;
                p[i].vz = po->v.z;

                points.push_back(p[i]);
                 ++i;

			}

}

void printResults(vector<Point>& points, int num_points)
{
    int i = 0;
    printf("Number of points: %u\n"
        " x     y     z     cluster_id\n"
        "-----------------------------\n"
        , num_points);
    while (i < num_points)
    {
          printf("%5.2lf %5.2lf %5.2lf: %d\n",
                 points[i].x,
                 points[i].y, points[i].z,
                 points[i].clusterID);
          ++i;
    }
}
void SaveResults(vector<Point>& points , int num_points){
    std::vector<std::vector<double> > particles;

    std::vector<double> colors;

    for(std::size_t i = 0 ; i< num_points ; ++i){
        particles.push_back(std::vector<double> {points[i].x,points[i].y, points[i].z});

        colors.push_back(points[i].clusterID);
    }

    double norm = ai::max(colors);
    std::cout<<"max cluster = "<<norm<<std::endl;

    for(std::size_t  i = 0 ; i< colors.size() ; ++i){
        colors[i] /= norm;
    }


    ai::saveXYZ(
        "out",
        particles,
        colors,
        "C"
    );
}

int main()
{
    vector<Point> points;

    // read point data
    // /readClusterData(points);
    std::string fname("./wr000_r000__ws008_s004_part_dump.dat");

    readDumpData( points ,fname);


    // constructor
    DBSCAN ds(MINIMUM_POINTS, EPSILON, points);

    // main loop
    ds.run();
    int idCluster = 1;

    std::vector<std::vector<double> > cluster;
    //find dispersion of speeds
     ds.FindDispersion( cluster, idCluster);

    // result of DBSCAN algorithm
    //printResults(ds.m_points, ds.getTotalPointSize());
    SaveResults(ds.m_points, ds.getTotalPointSize());

    return 0;
}
