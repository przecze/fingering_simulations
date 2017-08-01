#include<analyser.h>
#include<cstring>
#include<algorithm>

Tip::Tip() : x_max(0), x_min(0), y_max(0), y_min(0) {
}

void Tip::Add(int x, int y) {
  x_min = std::min(x_min, x);
  x_max = std::max(x_max, x);
  y_max = std::max(y_max, y);
  y_min = std::min(y_min, y);
}


Analyser::Analyser(
    std::istream& input_stream,
    std::ostream& output_stream
    ) : 
    input_stream_(input_stream),
    output_stream_(output_stream),
    front_position_(10),
    current_step_(0)
    {
}

void VPointBurned(i,j) {
  return v_(i,j)>burn_offset_;
}

void Analyser::FindTips() {
  int size_x = w_.size_x_;
  int size_y = w_.size_y_;
  Field f(w_.size_x_, w_size_y);
  f.Set(1.);
  for(int i = 0; i<size_x; ++i) {
    for(int j = 0; j<size_y; ++j) {
      if(

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
  std::cout<<"reading field: "<<field_name<<std::endl;
  int size_x;
  int size_y;
  input_stream_>>size_x>>size_y;
  if (!u_) {
    w_ = std::unique_ptr<Field>(new Field(size_x, size_y));
    v_ = std::unique_ptr<Field>(new Field(size_x, size_y));
    u_ = std::unique_ptr<Field>(new Field(size_x, size_y));
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
  for(int i = 0; i<size_x; ++i){
    for(int j = 0; j<size_y; ++j) {
      input_stream_>>field(i,j);
    }
  }

} 

bool Analyser::ReadStepLine() {
  std::string step_word;
  input_stream_>>step_word>>current_step_;
  std::cout<<"new step no: "<<current_step_<<std::endl;
  return input_stream_.good();
}

void Analyser::UpdateFrontPosition() {
  int next_front_position = front_position_;
  while(next_front_position<(w_->size_x_) && CheckIfLineBurned(next_front_position)){
    ++next_front_position;
  }
  if (next_front_position == w_->size_x_ - 1) {
    AnalyseFingers();
  }
  front_position_ = next_front_position;
  std::cout<<"new front pos: " <<front_position_<<std::endl;
}

void Analyser::PerformAnalysis() {
  while(current_step_<200000 ) {
    ReadStepLine();
    SkipFieldFromInput();
    SkipFieldFromInput();
    SkipFieldFromInput();
  }
  ReadStepLine();
  ReadFieldFromInput();
  ReadFieldFromInput();
  ReadFieldFromInput();
  std::string dummy;
  std::getline(input_stream_, dummy);
  UpdateFrontPosition();
  AnalyseTips();
  //AnalyseFingers();
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

  for (int i = 0; i<size_x; ++i) {
    bool last_burned = (((*v_)(i,0)>min_v_)?true:false);
    int tip_size = 0;

    for(int j = 0; j<size_y; ++j) {
        std::cout<<"tip "<<i<<", "<<j-tip_size/2<<std::endl;
      bool point_burned = (((*v_)(i,j)>min_v_)?true:false);
      if (point_burned){
        ++tip_size;
      }
      if (last_burned && !point_burned) {
        double ret  = CalculateTipFlow(i, j-tip_size/2, tip_size);
        std::cout<<"tip "<<i<<", "<<j-tip_size/2<<" flow: "<<ret<<std::endl;
        tip_size = 0;
      }
      last_burned  = point_burned;
    }
  }

}

double Analyser::CalculateTipFlow(int it, int jt, int r) {
  double result = 0;
  for (int i = it - r; i < it + r; ++i) {
    for (int j = jt - r; j < jt + r; ++j) {
      if ((i-it)*(i-it)+(j-jt)*(j-jt) < r*r) {
        result -= 4*(*u_)(i,j) - (*u_)(i - 1,j) - (*u_)(i + 1,j) - (*u_)(i,j-1) - (*u_)(i,j+1);
      }
    }
  }
  return result;
}

void Analyser::PrintStepLine(std::ostream& os) {
  os<<"step "<<current_step_<<'\n';
}

void Analyser::PrintAnalysedField(std::ostream& os) {
  u_->Print(os);
  v_->Print(os);
  w_->Print(os);
}
