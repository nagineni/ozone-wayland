// Copyright 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_IMPL_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_WAYLAND_H_
#define OZONE_IMPL_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_WAYLAND_H_

#include "base/compiler_specific.h"
#include "ui/views/views_export.h"
#include "ui/wm/public/drag_drop_client.h"

namespace views {

class VIEWS_EXPORT DesktopDragDropClientWayland
    : public aura::client::DragDropClient {
 public:
  explicit DesktopDragDropClientWayland(aura::Window* root_window);
  ~DesktopDragDropClientWayland() override;

  // Overridden from aura::client::DragDropClient:
  int StartDragAndDrop(const ui::OSExchangeData& data,
                       aura::Window* root_window,
                       aura::Window* source_window,
                       const gfx::Point& root_location,
                       int operation,
                       ui::DragDropTypes::DragEventSource source) override;
  void DragUpdate(aura::Window* target,
                  const ui::LocatedEvent& event) override;
  void Drop(aura::Window* target,
            const ui::LocatedEvent& event) override;
  void DragCancel() override;
  bool IsDragDropInProgress() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(DesktopDragDropClientWayland);
};

}  // namespace views

#endif  // OZONE_IMPL_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_WAYLAND_H_
