#include <iostream>
#include <vector>
#include "dispersion.hh"
#include "dbscan.h"
#include "ai.hh"


void DBSCAN::FindDispersion(std::vector<std::vector<double> >& cluster, int& idCluster){



    std::vector<Point>::iterator iter;
    int id=1;
    //std::size_t N = ai::max(iter->clusterID);
	
	std::vector<double> dispersion;
	
	std::vector<double> distances;

    while(id <= idCluster){

        for(iter = m_points.begin(); iter != m_points.end(); ++iter)
        {
            if ( iter->clusterID == id )
            {
                cluster.push_back(std::vector<double>{iter->x, iter->y,iter->z,std::sqrt(iter->vx*iter->vx + iter->vy * iter->vy+iter->vz*iter->vz)  } );
            }
        }
        //ai::printMatrix(cluster);
        ai::saveXYZ(ai::string("clust") + ai::string(id), cluster);

        double vmean = 0.;
        double xmean = 0.;
        double ymean = 0.;
        double zmean = 0.;
        double disp = 0.;
        double disppos = 0.;

        for(std::size_t i =0 ; i< cluster.size(); ++i){
            vmean += cluster[i][3];
            xmean += cluster[i][0];
            ymean += cluster[i][1];
            zmean += cluster[i][2];
        }

        vmean /= cluster.size();
        xmean /= cluster.size();
        ymean /= cluster.size();
        zmean /= cluster.size();

        double dx,dy,dz;
        std::vector<double> dist;

        for(std::size_t i =0 ; i < cluster.size();++i){
            for(std::size_t j=i+1 ; j<cluster.size() ;++j){
                dx = cluster[i][0] -cluster[j][0];
                dy = cluster[i][1] -cluster[j][1];
                dz = cluster[i][2] -cluster[j][2];

                dist.push_back(std::sqrt(dx*dx+dy*dy+dz*dz) );
            }
        }
        //ai::printVector(dist);
        double mindist = ai::min(dist);
        double distmean;

        for (std::size_t i =0; i< dist.size();++i){
            distmean += dist[i];
        }
        distmean /= dist.size();
		
		distances.push_back(distmean);
		
        std::cout<<"Cluster # "<<id<<std::endl;
        std::cout<<" minimum distance = "<<mindist<<"  mean distance = "<<distmean<<std::endl;
        for(std::size_t i =0 ; i< cluster.size(); ++i){
            disp += std::pow(cluster[i][3] - vmean , 2);
            disppos +=(cluster[i][0]-xmean)*(cluster[i][0]-xmean)+(cluster[i][1]-ymean)*(cluster[i][1]-ymean)+(cluster[i][2] - zmean)*(cluster[i][2] - zmean);
        }
        disp /= cluster.size();
        disppos /= cluster.size();

        std::cout<<"v mean = "<<vmean<<"   dispersion = "<<disp<<std::endl;
        std::cout<<"x mean = "<<xmean<<" y mean = "<<ymean<<" z mean = "<<zmean<<"   dispersion pos= "<<disppos<<std::endl;
        
		dispersion.push_back(disppos);
		
		std::vector<std::vector<double> > po;
        po.push_back(std::vector<double>{xmean, ymean,zmean});

        ai::saveXYZ(ai::string("mid") + ai::string(id), po);

        cluster.clear();
        po.clear();
        ++id;
    }
    //std::cout<<"vel"<<std::endl;
    //ai::printVector(vel);
    // double max = ai::max(vel);
    // double min = ai::min(vel);
    //
    // std::cout<<"max = "<<max<<std::endl;
    // std::cout<<"min = "<<min<<std::endl;
	double sumdi , sumde;
	for(size_t i = 0 ;i < dispersion.size(); ++i){
		
		sumde += dispersion[i];
		sumdi +=distances[i];
		
	}
	sumdi/= dispersion.size();
	sumde/=distances.size();
		
	std::cout<<"Mean Distances = "<<sumdi<<"   Mean Dispersion = "<<sumde<<std::endl;
		

}
