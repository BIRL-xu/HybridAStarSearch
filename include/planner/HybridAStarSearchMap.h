#include "planner/GridMap.h"
class HybridAStarSearchMap {
 public:
  explicit HybridAStarSearchMap(const std::string motion_planning_conf_path);

  std::shared_ptr<Node3d> CreateNodeFromWorldCoord(const double x,
                                                   const double y,
                                                   const double phi);
  std::shared_ptr<Node3d> GetNodeFromWorldCoord(const double x, const double y,
                                                const double phi);
  std::shared_ptr<Node3d> GetNodeFromGridCoord(const int x, const int y,
                                               const int phi);
  std::string Calc3dIndex(double x, double y, double phi);

  // initialization
  void SetXYResolution(double resolution);
  void SetPhiResolution(double resolution);
  void SetBounds(double xmin, double xmax, double ymin, double ymax);
  bool SetStartPoint(double x, double y, double phi);
  bool SetEndPoint(double x, double y, double phi);
  bool CheckStartEndPoints();
  void Reset();

  // search
  void AddObstacles(geometry_msgs::Polygon p);
  void AddObstacleArrayPtr(costmap_converter::ObstacleArrayMsgConstPtr ptr);
  bool GenerateHeuristicMap();
  bool Search();
  std::vector<common::PathPoint> GetResult(
      const udrive::common::VehicleState& vehicle);
  double GetObstacleDistance(std::shared_ptr<Node3d> p);
  double GetObstacleDistance(double x, double y);
  double ObstacleDistancePenalty(double dis);

  // state check
  bool InsideGridMap(const int node_grid_x, const int node_grid_y);
  bool InsideWorldMap(const double x, const double y);
  bool CollisionDection(double x, double y, double phi);

  // plot
  void PlotHeuristicMap();
  void PlotDebugMap();
  void PlotCostMap();
  void PlotTrajectory();

 private:
  // node expansion
  bool IsTerminateState(std::shared_ptr<Node3d> node);
  void Update(double& x, double& y, double& phi, double steer, double dis);
  void NextNodeGenerator(std::vector<std::shared_ptr<Node3d>>& next_nodes,
                         std::shared_ptr<Node3d>, double step_size);

  // heuristic map and search map
  GridMap grid_map_;
  std::unordered_map<std::string, std::shared_ptr<Node3d>> map_3d_;
  std::shared_ptr<Node3d> start_node_ = nullptr;
  std::shared_ptr<Node3d> end_node_ = nullptr;
  std::shared_ptr<Node3d> final_node_ = nullptr;

  // map params
  std::vector<double> XYbounds_{-10, 10, -10, 10};
  int max_grid_x_ = 0;
  int max_grid_y_ = 0;

  // visualization
  ros::NodeHandle nh;
  ros::Publisher pub, pub_cost_map;
  visualization_msgs::MarkerArray marker_array;
  visualization_msgs::Marker marker;

  // params nees to be set manually
  MotionPlanningConf motion_planning_conf_;
  double xy_grid_resolution_ = 0.3;
  double phi_grid_resolution_ = 0.2;
  int next_node_num_ = 5;
  double step_size_ = 0.02;
  double MAX_STEER = 0.8;
  double WHEEL_BASE = 2.0;
  double LIDAR_TO_REAR = 1.0;
  double VEHICLE_L = 2.0;
  double VEHICLE_W = 1.0;
  double WHEEL_BASE_TO_BACK = 0.7;

  // penalty
  double FORWARD_PENALTY = 1;
  double BACKWARD_PENALTY = 5;
  double STEER_PENALTY = 1;
  double STEER_CHANGE_PENALTY = 1;
  double OBSTACLE_PENALTY = 3;
  double SWITCH_PENALTY = 5.0;

  // counter
  double max_cost = std::numeric_limits<double>::min();
  int count = 0;
};