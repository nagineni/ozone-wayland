// Copyright 2014 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone/platform/ozone_wayland_window.h"

#include "ozone/ui/desktop_aura/desktop_screen_wayland.h"
#include "ozone/ui/desktop_aura/desktop_window_tree_host_ozone.h"
#include "ozone/ui/desktop_aura/window_tree_host_delegate_wayland.h"
#include "ozone/ui/events/window_state_change_handler.h"
#include "ui/platform_window/platform_window_delegate.h"

namespace ui {

views::WindowTreeHostDelegateWayland*
    OzoneWaylandWindow::g_delegate_ozone_wayland_ = NULL;

OzoneWaylandWindow::OzoneWaylandWindow(PlatformWindowDelegate* delegate,
                                       const gfx::Rect& bounds)
    : delegate_(delegate), bounds_(bounds) {
  static int opaque_handle = 0;
  opaque_handle++;
  handle_ = opaque_handle;
  delegate_->OnAcceleratedWidgetAvailable(opaque_handle);

  if (!g_delegate_ozone_wayland_)
    g_delegate_ozone_wayland_ = new views::WindowTreeHostDelegateWayland();

  g_delegate_ozone_wayland_->OnRootWindowCreated(this);
}

OzoneWaylandWindow::~OzoneWaylandWindow() {
}

void OzoneWaylandWindow::InitPlatformWindow(
    PlatformWindowType type, gfx::AcceleratedWidget parent_window) {
  ui::WindowStateChangeHandler* state_handler =
      ui::WindowStateChangeHandler::GetInstance();
  switch (type) {
    case PLATFORM_WINDOW_TYPE_TOOLTIP:
    case PLATFORM_WINDOW_TYPE_POPUP:
    case PLATFORM_WINDOW_TYPE_MENU: {
      // Wayland surfaces don't know their position on the screen and transient
      // surfaces always require a parent surface for relative placement. Here
      // there's a catch because content_shell menus don't have parent and
      // therefore we use root window to calculate their position.
      DCHECK(parent_window);
      OzoneWaylandWindow* active_window =
          g_delegate_ozone_wayland_->GetWindow(parent_window);

      DCHECK(active_window);
      gfx::Rect parent_bounds = active_window->GetBounds();
      // Don't size the window bigger than the parent, otherwise the user may
      // not be able to close or move it.
      // Transient type expects a position relative to the parent
      gfx::Point transientPos(bounds_.x() - parent_bounds.x(),
                              bounds_.y() - parent_bounds.y());

      state_handler->CreateWidget(handle_,
                                  active_window->GetHandle(),
                                  transientPos.x(),
                                  transientPos.y(),
                                  ui::POPUP);
      break;
    }
    case PLATFORM_WINDOW_TYPE_BUBBLE:
    case PLATFORM_WINDOW_TYPE_WINDOW:
      state_handler->CreateWidget(handle_,
                                  0,
                                  0,
                                  0,
                                  ui::WINDOW);
      break;
    case PLATFORM_WINDOW_TYPE_WINDOW_FRAMELESS:
      NOTIMPLEMENTED();
      break;
    default:
      break;
  }
}

gfx::Rect OzoneWaylandWindow::GetBounds() {
  return bounds_;
}

void OzoneWaylandWindow::SetBounds(const gfx::Rect& bounds) {
  bounds_ = bounds;
  delegate_->OnBoundsChanged(bounds_);
}

void OzoneWaylandWindow::Show() {
  ui::WindowStateChangeHandler::GetInstance()->SetWidgetState(handle_,
                                                              ui::SHOW);
}

void OzoneWaylandWindow::Hide() {
  ui::WindowStateChangeHandler::GetInstance()->SetWidgetState(handle_,
                                                              ui::HIDE);
}

void OzoneWaylandWindow::Close() {
  g_delegate_ozone_wayland_->OnRootWindowClosed(this);
  if (!g_delegate_ozone_wayland_->HasWindowsOpen()) {
    // We have no open windows, free g_delegate_ozone_wayland_.
    delete g_delegate_ozone_wayland_;
    g_delegate_ozone_wayland_ = NULL;
  }
}

void OzoneWaylandWindow::SetCapture() {
}

void OzoneWaylandWindow::ReleaseCapture() {
}

void OzoneWaylandWindow::ToggleFullscreen() {
  gfx::Screen *screen = gfx::Screen::GetScreenByType(gfx::SCREEN_TYPE_NATIVE);
  if (!screen)
    NOTREACHED() << "Unable to retrieve valid gfx::Screen";

  SetBounds(screen->GetPrimaryDisplay().bounds());
  ui::WindowStateChangeHandler::GetInstance()->SetWidgetState(handle_,
                                                              ui::FULLSCREEN);
}

void OzoneWaylandWindow::Maximize() {
  ui::WindowStateChangeHandler::GetInstance()->SetWidgetState(handle_,
                                                              ui::MAXIMIZED);
}

void OzoneWaylandWindow::Minimize() {
  SetBounds(gfx::Rect());
  ui::WindowStateChangeHandler::GetInstance()->SetWidgetState(handle_,
                                                              ui::MINIMIZED);
}

void OzoneWaylandWindow::Restore() {
  ui::WindowStateChangeHandler::GetInstance()->SetWidgetState(handle_,
                                                              ui::RESTORE);
}

void OzoneWaylandWindow::SetCursor(PlatformCursor cursor) {
}

void OzoneWaylandWindow::MoveCursorTo(const gfx::Point& location) {
}

}  // namespace ui
