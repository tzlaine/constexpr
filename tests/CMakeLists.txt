find_package(Catch2 3 REQUIRED)

add_executable(test-runner
	basic.cpp basic2.cpp
)

target_link_libraries(test-runner PRIVATE Catch2::Catch2WithMain constant_wrapper)
target_compile_features(test-runner PUBLIC cxx_std_26)

add_custom_target(test test-runner --skip-benchmarks --colour-mode ansi "" DEPENDS test-runner)
add_custom_target(long-test test-runner --skip-benchmarks --colour-mode ansi "*,[.long]" DEPENDS test-runner)
add_custom_target(benchmark test-runner --colour-mode ansi "" DEPENDS test-runner)

coverage_report_after(test test-runner)
