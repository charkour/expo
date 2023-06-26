/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <shared_mutex>

#include <ABI49_0_0butter/ABI49_0_0map.h>

#include <ABI49_0_0React/renderer/core/ABI49_0_0ReactPrimitives.h>
#include <ABI49_0_0React/renderer/mounting/ABI49_0_0ShadowTree.h>

namespace ABI49_0_0facebook {
namespace ABI49_0_0React {

/*
 * Owning registry of `ShadowTree`s.
 */
class ShadowTreeRegistry final {
 public:
  ShadowTreeRegistry() = default;
  ~ShadowTreeRegistry();

  /*
   * Adds a `ShadowTree` instance to the registry.
   * The ownership of the instance is also transferred to the registry.
   * Can be called from any thread.
   */
  void add(std::unique_ptr<ShadowTree> &&shadowTree) const;

  /*
   * Removes a `ShadowTree` instance with given `surfaceId` from the registry
   * and returns it as a result.
   * The ownership of the instance is also transferred to the caller.
   * Returns `nullptr` if a `ShadowTree` with given `surfaceId` was not found.
   * Can be called from any thread.
   */
  std::unique_ptr<ShadowTree> remove(SurfaceId surfaceId) const;

  /*
   * Finds a `ShadowTree` instance with a given `surfaceId` in the registry and
   * synchronously calls the `callback` with a reference to the instance while
   * the mutex is being acquired.
   * Returns `true` if the registry has `ShadowTree` instance with corresponding
   * `surfaceId`, otherwise returns `false` without calling the `callback`.
   * Can be called from any thread.
   */
  bool visit(
      SurfaceId surfaceId,
      std::function<void(const ShadowTree &shadowTree)> const &callback) const;

  /*
   * Enumerates all stored shadow trees.
   * Set `stop` to `true` to interrupt the enumeration.
   * Can be called from any thread.
   */
  void enumerate(
      std::function<void(const ShadowTree &shadowTree, bool &stop)> const
          &callback) const;

 private:
  mutable std::shared_mutex mutex_;
  mutable butter::map<SurfaceId, std::unique_ptr<ShadowTree>>
      registry_; // Protected by `mutex_`.
};

} // namespace ABI49_0_0React
} // namespace ABI49_0_0facebook
