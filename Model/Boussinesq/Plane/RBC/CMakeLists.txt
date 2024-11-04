target_sources(${QUICC_CURRENT_MODEL_LIB} ${QUICC_CMAKE_SRC_VISIBILITY}
  IRBCModel.cpp
  IRBCBackend.cpp
  Momentum.cpp
  MomentumKernel.cpp
  Transport.cpp
  TransportKernel.cpp
  )

add_subdirectory(Explicit)
