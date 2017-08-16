#include<analyser.h>
#include<cstring>
#include<iomanip>
#include<algorithm>
#include<cmath>

Tip::Tip(int i, int j) :
    x_max(i), x(i), x_min(i),
    y_max(j), y(j), y_min(j)
    {
}

void Tip::Add(int i, int j, int size_y) {
  x_min = std::min(x_min, i);
  x_max = std::max(x_max, i);
  y_max = std::max(y_max, j);
  y_min = std::min(y_min, j);
  x = (x_min + x_max)/2;
  y = (y_min + y_max)/2;
}

void Tip::Fix(int size_y) {
  y = (y+size_y)%size_y;
  y_min = (y_min+size_y)%size_y;

}

int Tip::Width() {
  return y_max -y_min;
}

int Tip::Dist(const Tip& tip, int size_y) {
  int y_dist = std::abs(tip.y-y);
  y_dist = std::min(y_dist, size_y - y_dist);
  return (tip.x-x)*(tip.x-x) + y_dist*y_dist;
}

void PrintLaplInfoFormat(std::ostream& output_stream_) {
  output_stream_<<"tips in format: (tip_no,pos_y, lapl, laplx, laply)"<<'\n';
}

void Tip::PrintLaplInfo(std::ostream& output_stream_) {
    output_stream_<<'('<<std::setw(2)<<num<<','
                  <<std::setw(3)<<y<<", "
                  <<std::setprecision(2)<<lapl<<", "
                  <<std::setprecision(2)<<lapl_x<<", "
                  <<std::setprecision(2)<<lapl_y<<") ";
}

void PrintFlowInfoFormat(std::ostream& output_stream_) {
  output_stream_<<"tips in format: (tip_no,pos_y, flow_ang, flow)"<<'\n';
}

void Tip::PrintFlowInfo(std::ostream& output_stream_) {
    output_stream_<<'('<<std::setw(2)<<num<<','
                  <<std::setw(3)<<y<<", "
                  <<std::setprecision(2)<<flow_ang<<", "
                  <<std::setprecision(2)<<flow<<") ";
}

void PrintVelInfoFormat(std::ostream& output_stream_) {
  output_stream_<<"tips in format: (tip_no,pos_y, vel, velx, vely)"<<'\n';
}

void Tip::PrintVelInfo(std::ostream& output_stream_) {
    output_stream_<<'('<<std::setw(2)<<num<<','
                  <<std::setw(3)<<y<<", "
                  <<std::setw(3)<<vel<<", "
                  <<std::setw(2)<<vel_x<<", "
                  <<std::setw(2)<<vel_y<<") ";
}

void Tip::CustomPrint(std::ostream& o) {
  o<<num<<' '
   <<(parent?parent->num:-1)<<' '
   <<x<<' '
   <<y<<' '
   <<vel_x<<' '
   <<vel_y<<' '
   <<lapl<<' '
   <<vel<<' '
   <<flow<<' '
   ;
}


Analyser::Analyser(
    std::istream& input_stream,
    std::ostream& output_stream,
    OutputType type
    ) : 
    input_stream_(input_stream),
    output_stream_(output_stream),
    output_type_(type),
    front_position_(10),
    current_step_(0),
    tip_num_(0)
    {
}

bool Analyser::VPointBurned(int i, int j) {
  return (*v_)(i,j)>min_v_;
}

void Analyser::SortTips() {
  std::sort(tips_.begin(), tips_.end(),
      [](const Tip& a, const Tip& b)->bool {
        return a.y < b.y;
      });
}

void Analyser::FindTips() {
  int size_x = w_->size_x_;
  int size_y = w_->size_y_;
  int x_to_start;
  if (old_tips_.size() != 0) {
    auto min_x_tip = std::min_element(std::begin(old_tips_),
                                      std::end(old_tips_),
                                      [](const Tip& a, const Tip& b) {
                                      return a.x < b.x;
                                      });
    x_to_start = min_x_tip->x;
  } else {
    x_to_start = 5;
  }

  tips_.clear();
  Field marked(size_x,size_y);
  marked.SetValue(1.);
  std::cout<<"check form "<<x_to_start<<" to "<<std::min(front_position_, size_x-5)<<std::endl;
  for(int i = x_to_start; i<std::min(front_position_, size_x-5); ++i) {
    for(int j = 0; j<size_y; ++j) {
      if(marked(i,j) ==1 && VPointBurned(i,j)) {
        std::cout<<"tip found at "<<i<<" "<<j<<std::endl;
        tips_.push_back(Tip(i,j));
        Mark(marked, tips_.back(), i, j);
      }
    }
  }
  if (tips_.size() == 0) {
    std::cout<<"Analyser: no tips found!"<<std::endl;
    simulation_ended_ = true;
  }

}

void Analyser::Mark(Field &marked, Tip &tip, int i, int j) {
  marked(i, j) = 0;
  tip.Add(i,j, v_->size_x_);
  if ((*v_)(i,j)>tip.max_v) {
    tip.max_v_x = i;
    tip.max_v_y = j;
  }
  int ni;
  int nj;
  for (int magic = 0; magic < 4; ++magic) {
    ni = i + magic/2 - magic%2;
    nj = j - 1 + magic/2 + magic%2;
    if (marked(ni,nj)==1 && VPointBurned(ni,nj)) {
      Mark(marked, tip, ni, nj);
    }
  }
}

void Analyser::PrintTips() {
  for(auto& tip : tips_) {
    output_stream_<<current_step_<<' ';
    tip.CustomPrint(output_stream_);
    output_stream_<<'\n';
  }
  output_stream_<<std::flush;
}

bool Analyser::CheckIfLineBurned(int line_num) {
  for(int j = 0; j<w_->size_y_; ++j) {
    if((*w_)(line_num, j)<burn_offset_) {
        return true;
    }
  }
  return false;

}

void Analyser::RewriteData(){
  output_stream_<<input_stream_.rdbuf();
}

void Analyser::SkipFieldFromInput() {
  int size_x;
  int size_y;
  char field_name;
  input_stream_>>field_name;
  input_stream_>>size_x>>size_y;
  if (!w_) {
    w_ = std::unique_ptr<Field>(new Field(size_x, size_y));
  }
  std::string s;
  for(int i = 0; i<size_x+1; ++i) {
    std::getline(input_stream_, s);
  }

} 

void Analyser::ReadFieldFromInput() {
  char field_name;
  input_stream_>>field_name;
  input_stream_>>size_x_>>size_y_;
  if (!u_) {
    w_ = std::unique_ptr<Field>(new Field(size_x_, size_y_));
    v_ = std::unique_ptr<Field>(new Field(size_x_, size_y_));
    u_ = std::unique_ptr<Field>(new Field(size_x_, size_y_));
  }
  Field* ptr;
  if (field_name=='u') {
    ptr = u_.get();
  }
  if (field_name=='v') {
    ptr = v_.get();
  }
  if (field_name=='w') {
    ptr = w_.get();
  }
  Field& field = *ptr;
  for(int i = 0; i<size_x_; ++i){
    for(int j = 0; j<size_y_; ++j) {
      input_stream_>>field(i,j);
    }
  }

} 

bool Analyser::ReadStepLine() {
  std::string step_word;
  input_stream_>>step_word>>current_step_;
  return input_stream_.good();
}

void Analyser::UpdateFrontPosition() {
  int next_front_position = front_position_;
  while(next_front_position<(w_->size_x_) && CheckIfLineBurned(next_front_position)){
    ++next_front_position;
  }
  if (next_front_position > w_->size_x_ - r_for_lapl_calculation_ - 3) {
    std::cout<<"Front reached the end";
    simulation_ended_ = true;
    OnEnd();
  }
  front_position_ = next_front_position;
  //std::cout<<"new front pos: " <<front_position_<<std::endl;
}

void Analyser::SkipToStep(int step) {
  while(current_step_<step && ReadStepLine()) {
    //std::cout<<"skip"<<std::endl;
    //PrintStepLine(std::cout);
    SkipFieldFromInput();
    SkipFieldFromInput();
    SkipFieldFromInput();
  }
  if(input_stream_.bad()) {
    std::cout<<"Analyser::SkipToStep("<<step<<"): stream ended at "<<current_step_<<std::endl;
  }
}

bool Analyser::LoadFields() {
  bool result = ReadStepLine();
  if(result) {
    ReadFieldFromInput();
    ReadFieldFromInput();
    ReadFieldFromInput();
  } else {
    std::cout<<"Analyser::LoadFields : stream ended, unable to load"<<std::endl;
  }
  //UpdateFrontPosition();
  //AnalyseFingers();
  return result;
}

void Analyser::PerformAnalysis(int start_step, int end_step) {
  if (start_step != 0 ) { 
    SkipToStep(start_step);
  }
  while(current_step_ < end_step && LoadFields()) {
    UpdateFrontPosition();
    //PrintStepLine(output_stream_);
    FindTips();
    AnalyseTips();
    //PrintAvgLapl();
    //PrintMinValue();
    //RawPrint();
    output_stream_<<std::flush;
    PrintTips();
    SortTips();
    old_tips_ = tips_;
  }
}

void Analyser::Step() {
  if (LoadFields()) {
    UpdateFrontPosition();
    FindTips();
    AnalyseTips();
    if (output_type_ == kTipsData) {
      PrintTips();
    }
    SortTips();
    old_tips_ = tips_;
  } else {
    std::cout<<"Analyser: cant perform analysis step. Not enough data in stream"<<std::endl;
    simulation_ended_ = true;
    OnEnd();
  }
}

void Analyser::OnEnd() {
  if (output_type_ == kMetaData) {
    output_stream_ << AvgFrontVelocity()<<' '
                   << AvgTipsDist() << ' '
                   << AvgTipsWidth() << ' '
                   ;
    output_stream_<<std::endl;
  }
}


void Analyser::AnalyseFingers() {
  int size_y = w_->size_y_;
  int size_x = w_->size_x_;
  for (int i = 0; i<size_x; ++i) {
    int finger_count = 0;
    int burn_count = 0;
    bool last_burned = (((*w_)(i,0)<burn_offset_)?true:false);
    for(int j = 0; j<size_y; ++j) {
      bool point_burned = (((*w_)(i,j)<burn_offset_)?true:false);
      burn_count += point_burned;
      if (last_burned != point_burned) {
        ++finger_count;
        last_burned = point_burned;
      }
    }

    if (last_burned != (((*w_)(i,0)<burn_offset_)?true:false)) {
      ++finger_count;
      std::cout<<"finger at edge"<<std::endl;
    }
    finger_count/=2;
    int finger_width;
    int finger_dist;

    if(finger_count) {
      finger_width = burn_count/finger_count;
      finger_dist = size_y/finger_count;
    } else {
      finger_width = -1;
      finger_dist = -1;
    }
    
    if (finger_count%2==1) {
      std::cout<<"odd finger count!"<<finger_count<<std::endl;
    } else {
      std::cout<<i<<" "<<finger_count<<" "<<finger_width<<" "<<finger_dist<<std::endl;
    }
  }
}

void Analyser::AnalyseTips() {
  int size_y = v_->size_y_;
  int size_x = v_->size_x_;
  bool bifurcation_occured = false;
  tips_count_.push_back(tips_.size());
  for (auto& tip : tips_) {
    tips_width_.push_back(tip.Width());
    CalculateValuesForTip(tip);
    Tip* nearest = nullptr;
    int min_dist = size_x*size_x*4;
    for (auto& old_tip : old_tips_) {
      int dist = tip.Dist(old_tip, size_x);
      if (dist<min_dist) {
        nearest = &old_tip;
        min_dist = dist;
        //std::cout<<"nearest found "<<nearest->num<<" dist "<<dist<<std::endl;
      }
    }
    tip.parent = nearest;
    if (tip.parent!=nullptr) {
      tip.num = tip.parent->num;
      tip.vel_x = tip.x - tip.parent->x;
      tip.vel_y = tip.y - tip.parent->y;
      tip.vel = min_dist;
      if(min_dist > 1000) {
        std::cout<<"dist!!!"<<std::endl;
        std::cout<<tip.x<<" "<<tip.y<<" "<<tip.parent->x<<" "<<tip.parent->y<<std::endl;
    }
      if(tip.parent->has_child) {
        bifurcation_occured = true;
        tip.parent->bifurcated=true;
        tip.num = ++tip_num_;
        tip.parent->child_num = tip.num;
        //output_stream_<<"bifurcation of "<<tip.parent->num
        //              <<" while vel="<<std::setw(3)<<tip.parent->vel
        //              <<" (added "<<tip.num<<")"
        //              <<'\n'<<std::flush;
      } else {
        tip.parent->has_child = true;
      }
      //auto angle = std::atan2((tip.y-tip.parent->y) , (tip.x - tip.parent->x));
      //output_stream_<<"Tip "<<tip.num<<" grow_ang: "<<angle<<std::flush;
      //auto angle2 = tip.parent->vel_ang;
      //output_stream_<<" vs "<<angle2<<'\n';

    } else {
      //output_stream_<<"Tip without parent found at "<<tip.x<<" "<<tip.y<<'\n';
      tip.num = ++tip_num_;
    }
  }
}

void Analyser::PrintAvgLapl() {
  double avg_lapl = 0;
  for(auto& tip : old_tips_) {
    if (!tip.bifurcated) {
      avg_lapl += tip.vel;
    }
  }
  if (old_tips_.size()!=0) {
    avg_lapl/=old_tips_.size();
  }
  output_stream_<<"avg_vel of non-spliting tips: "<<avg_lapl<<'\n';
}

void Analyser::PrintMaxLapl() {
  double max_lapl = -1000;
  for(auto& tip : old_tips_) {
    if (!tip.bifurcated) {
      max_lapl = std::max(max_lapl, tip.vel);
    }
  }
  output_stream_<<"max_vel of non-spliting tips: "<<max_lapl<<'\n';
}

void Analyser::PrintMinValue() {
  double min_value = 10000;
  for(auto& tip : old_tips_) {
    if (!tip.bifurcated) {
      min_value = std::min(min_value, tip.vel);
    }
  }
  output_stream_<<"min_vel of non-spliting tips: "<<min_value<<'\n';
}

void Analyser::CalculateValuesForTip(Tip& tip) {
  tip.flow_x = 0;
  tip.flow_y = 0;
  int it = (tip.x_min + tip.x_max)/2;
  int jt = (tip.y_min + tip.y_max)/2;
  int r = r_for_lapl_calculation_;
  int r2 = r*r;
  Field& U = *u_;
  for (int i = it - r; i < it + r; ++i) {
    for (int j = jt - r; j < jt + r; ++j) {
      if ((i-it)*(i-it)+(j-jt)*(j-jt) < r2) {
        double weight = U(it,jt) - U(i,j);
        tip.flow_x += weight * (it-i);
        tip.flow_y += weight * (jt-j);
        tip.lapl_x += U(i - 1,j) + U(i + 1,j) - 2*U(i,j);
        tip.lapl_y += U(i,j-1) + U(i,j+1) - 2*U(i,j);
      }
    }
  }
  tip.lapl = tip.lapl_x+tip.lapl_y;
  tip.lapl_ratio = tip.lapl_x/tip.lapl_y;
  tip.flow_ang = atan2(tip.flow_y, tip.flow_x);
  tip.flow = tip.flow_y*tip.flow_y + tip.flow_x*tip.flow_x;
  tip.fuel = (*w_)(tip.x, tip.y);
  if (max_v_as_center_) {
    tip.x = tip.max_v_x;
    tip.y = tip.max_v_y;
  }
  tip.Fix(size_y_);
}

void Analyser::PrintStepLine(std::ostream& os) {
  os<<"step "<<current_step_<<'\n';
}

void Analyser::PrintAnalysedField(std::ostream& os) {
  u_->Print(os);
  v_->Print(os);
  w_->Print(os);
}

void Analyser::RawPrint() {
//  for(auto& tip : tips_) {
//    tip* parent = (tip.parent?tip.parent:(new tip(0,0)));
//    output_stream_<<tip.vel
//                  <<' '<<parent->flow
//                  <<' '<<parent->flow_ang
//                  <<' '<<parent->flow_x
//                  <<' '<<parent->flow_y
//                  <<' '<<parent->lapl
//                  <<' '<<parent->lapl_ratio
//                  <<' '<<parent->lapl_x
//                  <<' '<<parent->lapl_y
//                  <<'\n';
//  }
  for(auto& tip : tips_) {
    output_stream_<<current_step_<<' '<<tip.lapl<<'\n';
  }
  output_stream_<<std::flush;
}

double Analyser::AvgFrontVelocity() {
  if (current_step_!=0) {
    return front_position_/double(current_step_);
  } else {
    return -1;
  }
}

double Analyser::AvgTipsDist() {
  double avg_tip_count;
  if ( tips_count_.size() != 0) {
    avg_tip_count =
        std::accumulate(tips_count_.begin(), tips_count_.end(), 0LL)/double(tips_count_.size());
  } else {
    avg_tip_count = -1;
  }

  return double(size_x_)/avg_tip_count;
}

double Analyser::AvgTipsWidth() {
  double avg_tip_width;
  if ( tips_width_.size() != 0) {
    avg_tip_width =
        std::accumulate(tips_width_.begin(), tips_width_.end(), 0LL)/double(tips_width_.size());
  } else {
    avg_tip_width = 0.;
  }

  return avg_tip_width;
}
