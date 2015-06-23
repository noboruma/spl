namespace spl 
{
  namespace io 
  {
    namespace csv 
    {
      template<typename S>
      void save(const std::string& file, const NDSignal<S>& in)
      {
        std::ofstream out(file, std::ios::out);
        traits_iterator_decl(in, it);
        for_each_element(it)
          out << in[it] <<", ";
      }

      template<typename S>
      void load(const std::string& file, NDSignal<S>& out)
      {
        std::ifstream in(file, std::ios::in);
        traits_iterator_decl(out, it);
        char delim;
        for_each_element(it)
        {
          in >> out[it];
          in >> delim;;
        }
      }
    } //!csv

    namespace raw 
    {
      template<typename S>
      void save(const std::string& file, const NDSignal<S>& in)
      {
        std::ofstream out(file, std::ios::out | std::ios::binary );
        traits_iterator_decl(in, it);
        //unsafe
        out.write((char*)&in[it], in.domain().prod()*sizeof(traits_value_type(S)));
      }

      template<typename S>
      void load(const std::string& file, NDSignal<S>& out)
      {
        std::ifstream in(file, std::ios::in | std::ios::binary );
        traits_iterator_decl(out, it);
        //unsafe
        in.read((char*)&out[it], out.domain().prod()*sizeof(traits_value_type(S)));
      }
    } //!raw
  } //!io
} //!spl
