

struct hydra_thread : public hydra_object {
  std::string to_string() const;
};

struct hydra_mutex : public hydra_object {
  std::string to_string() const;
};

struct hydra_semaphore : public hydra_object {
  std::string to_string() const;
};
