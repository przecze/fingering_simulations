#include<analyser.h>
#include<cstring>


Analyser::Analyser(
    std::istream& input_stream,
    std::ostream& output_stream
    ) : 
    input_stream_(input_stream),
    output_stream_(output_stream),
    front_position_(10)
    {
}

bool Analyser::CheckIfLineBurned(int line_num) {
  for(int j = 0; j<analysed_field_->size_y_; ++j) {
    if((*analysed_field_)(line_num, j)<burn_offset_) {
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
  if (!analysed_field_) {
    analysed_field_ = std::unique_ptr<Field>(new Field(size_x, size_y));
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
  if (!analysed_field_) {
    analysed_field_ = std::unique_ptr<Field>(new Field(size_x, size_y));
  }
  for(int i = 0; i<size_x; ++i){
    for(int j = 0; j<size_y; ++j) {
      input_stream_>>(*analysed_field_)(i,j);
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
  while(front_position_<(analysed_field_->size_x_) && CheckIfLineBurned(front_position_)){
    ++front_position_;
  }
}

void Analyser::PerformAnalysis() {
  while(ReadStepLine()) {
    SkipFieldFromInput();
    SkipFieldFromInput();
    ReadFieldFromInput();
    std::string dummy;
    std::getline(input_stream_, dummy);
    UpdateFrontPosition();
    std::cout<<current_step_<<" "<<front_position_<<'\n';
    output_stream_<<current_step_<<" "<<front_position_<<std::endl;
  }
}

void Analyser::PrintAnalysedField() {
  analysed_field_->Print(std::cout);
}
