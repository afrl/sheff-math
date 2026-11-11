# SHEFF Math (`sheff-math`)

`sheff-math` (`SimpleMath::simple_math`) provides dense linear algebra wrappers, spatial geometry primitives, Voigt tensor mechanics helpers, combinatorial matching, and numerical quadrature rules for the **Scalable Hierarchical Enrichment Finite-element Framework (SHEFF)**.

## Capabilities

- **Dense Linear Algebra & Block Structures**:
  - Eigen3 type aliases and utilities (`EigenDefines.h`, `EigenUtilities.h`), preallocated heap workspaces (`HeapPreallocatedMatrix.h`), and block dense containers (`BlockMatrix.h`, `BlockVector.h`).
  - Continuum mechanics Voigt notation conversions (`VoigtHelpers.h`) and 3D orientation/rotation utilities (`QuaternionHelpers.h`).
- **Computational Geometry & Spatial Search**:
  - 1D/2D/3D geometric primitives and intersections (`LineGeometry.h`, `PlaneGeometry.h`, `Intersection.h`, `ConvexHull.h`), spline curve interpolation (`SplineLine.h`), and header-only K-D tree nearest-neighbor search (`external/nanoflann.h`).
  - Minimum-weight bipartite matching via the Hungarian algorithm (`HungarianAlgorithm.h`).
- **Numerical Quadrature (`simple_math/quadrature/`)**:
  - Standard and high-order Gauss-Legendre and Newton-Cotes integration rules in 1D, 2D, and 3D (`Gauss1D.h`, `Gauss2D.h`, `Gauss3D.h`, `GaussTriangle.h`, `GaussTetrahedron.h`, `GaussWedge.h`, `NewtonCotes1D.h`, `NewtonCotes2D.h`, `NewtonCotes3D.h`).
  - Unified `Quadrature` interface and `AdaptiveQuadratureData` structures supporting sub-divided / enriched finite elements.

## Project Structure

- `include/simple_math/`: Public headers for dense matrices, geometry, Voigt/quaternion helpers, and `quadrature/`.
- `source/`: Compiled implementations for quadrature tables, geometric intersections, splines, and combinatorial algorithms.
- `unit_tests/`: Unit tests for quadrature accuracy, geometric intersections, splines, and dense matrix utilities.
- **CMake Package / Target**: `SimpleMath` / `SimpleMath::simple_math` (static library).

## Dependencies

- **Build System**: [`sheff-build-tools`](https://github.com/afrl/sheff-build-tools) (`SHEFF_TOOLS`), CMake 3.17+, C++17/C++20 compiler.
- **First-Party SHEFF Modules**: [`sheff-utility`](https://github.com/afrl/sheff-utility) (`Utility::utility`).
- **Third-Party Libraries**: [`Eigen3`](https://github.com/eigenteam/eigen-git-mirror) (`Eigen3::Eigen`).

## Building

When building as part of [SPAWC](https://github.com/afrl/spawc), `sheff-math` is configured and built automatically via `make.py`.

To build and test standalone (with `SHEFF_TOOLS` configured):

```bash
python3 make.py --config --build --third-party
python3 make.py --config --build --make-tests --run-tests
```

## Public Release Information

Distribution Statement A. Approved for public release: distribution is unlimited. Case #: AFRL-2026-4513
