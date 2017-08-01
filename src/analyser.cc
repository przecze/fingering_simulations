#include<analyser.h>
#include<cstring>
#include<iomanip>
#include<algorithm>

Tip::Tip(int i, int j) :
    x_max(i), x_min(i),
    y_max(j), y_min(j),
    parent(nullptr) {
}

void Tip::Add(int i, int j) {
  x_min = std::min(x_min, i);
  x_max = std::max(x_max, i);
  y_max = std::max(y_max, j);
  y_min = std::min(y_min, j);
  x = (x_min + x_max)/2;
  y = (y_min + y_max)/2;
  
}

int Tip::Dist(const Tip& tip, int size_x) {
  int x_dist = std::max(tip.x-x, x-tip.x);
  x_dist = std::min(x_dist, size_x - x_dist);
  return (tip.y-y)*(tip.y-y) + x_dist*x_dist;
}


Analyser::Analyser(
    std::istream& input_stream,
    std::ostream& output_stream
    ) : 
    input_stream_(input_stream),
    output_stream_(output_stream),
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
  tips_.clear();
  Field marked(size_x,size_y);
  marked.SetValue(1.);
  for(int i = 0; i<size_x; ++i) {
    for(int j = 0; j<size_y; ++j) {
      if(marked(i,j) ==1 && VPointBurned(i,j)) {
        tips_.push_back(Tip(i,j));
        Mark(marked, tips_.back(), i, j);
      }
    }
  }
}

void Analyser::Mark(Field &marked, Tip &tip, int i, int j) {
  marked(i, j) = 0;
  tip.Add(i,j);
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
    //std::cout<< "Tip: " <<tip.x_min << " to " <<tip.x_max 
    //          << ", " <<tip.y_min << " to " <<tip.y_max<<std::endl;
    output_stream_<<std::setw(2)<<tip.num<<' '
        <<'('<<tip.x<<','<<tip.y<<')'<<' ';
  }
  /*
  output_stream_<<std::endl;
  for(auto& tip : tips_) {
    //std::cout<< "Tip: " <<tip.x_min << " to " <<tip.x_max 
    //          << ", " <<tip.y_min << " to " <<tip.y_max<<std::endl;
    output_stream_<<std::setw(11)<<std::setprecision(4)<<tip.lapl<<' ';
  }
  */
  output_stream_<<std::endl;
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
  //std::cout<<"reading field: "<<field_name<<std::endl;
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
  //std::cout<<"new step no: "<<current_step_<<std::endl;
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

void Analyser::SkipToStep(int step) {
  while(current_step_<step && ReadStepLine()) {
    std::cout<<"skip"<<std::endl;
    PrintStepLine(std::cout);
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

void Analyser::PerformAnalysis(int start_step) {
  if (start_step != 0 ) {
    SkipToStep(start_step);
  }
  std::cout<<"irer"<<std::endl;
  while(LoadFields()) {
    //UpdateFrontPosition();
    //FindTips();
    FindTips();
    AnalyseTips();
    SortTips();
    PrintTips();
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
  for (auto& tip : tips_) {
    tip.lapl = CalculateTipLaplace(tip);
    tip.flow = CalculateTipFlow(tip);
    Tip* nearest = nullptr;
    int min_dist = 2*size_x;
    for (auto& old_tip : old_tips_) {
      int dist = tip.Dist(old_tip, size_x);
      if (dist<min_dist) {
        nearest = &old_tip;
        min_dist = min_dist;
      }
    }
    tip.parent = nearest;
    if (tip.parent!=nullptr) {
      tip.num = tip.parent->num;
    } else {
      tip.num = ++tip_num_;
    }
  }
  old_tips_ = tips_;
}

double Analyser::CalculateTipLaplace(Tip& tip) {
  double result = 0;
  int it = (tip.x_min + tip.x_max)/2;
  int jt = (tip.y_min + tip.y_max)/2;
  int r = (tip.x_max - tip.x_min)/2;
  for (int i = it - r; i < it + r; ++i) {
    for (int j = jt - r; j < jt + r; ++j) {
      if ((i-it)*(i-it)+(j-jt)*(j-jt) < r*r) {
        result -= 4*(*u_)(i,j) - (*u_)(i - 1,j) - (*u_)(i + 1,j) - (*u_)(i,j-1) - (*u_)(i,j+1);
      }
    }
  }
  return result;
}

double Analyser::CalculateTipFlow(Tip& tip) {
  double resultx = 0;
  double resulty = 0;
  int it = (tip.x_min + tip.x_max)/2;
  int jt = (tip.y_min + tip.y_max)/2;
  int r = (tip.x_max - tip.x_min)/2;
  Field& U = *u_;
  for (int i = it - r; i < it + r; ++i) {
    for (int j = jt - r; j < jt + r; ++j) {
      if ((i-it)*(i-it)+(j-jt)*(j-jt) < r*r) {
        double weight = U(it,jt) - U(i,j);
        resultx += weight * (i-it);
        resulty += weight * (j-jt);
      }
    }
  }
  return resultx/resulty;
}

void Analyser::PrintStepLine(std::ostream& os) {
  os<<"step "<<current_step_<<'\n';
}

void Analyser::PrintAnalysedField(std::ostream& os) {
  u_->Print(os);
  v_->Print(os);
  w_->Print(os);
}
