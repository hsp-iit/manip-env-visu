#ifndef MANIP_ENV_H
#define MANIP_ENV_H

#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <fstream>
#include <stdio.h>
#include <filesystem>

#include "gripper.h"

namespace mev
{
    class ManipulationEnv
    {
    private:
        std::string background_color;
        vtkNew<vtkNamedColors> colors;
        std::vector<vtkSmartPointer<vtkAxesActor>> axes_list;
        vtkSmartPointer<vtkRenderer> renderer;
        vtkSmartPointer<vtkRenderWindow> render_window;
        vtkSmartPointer<vtkRenderWindowInteractor> window_interactor;
        vtkSmartPointer<vtkInteractorStyleTrackballCamera> window_interactor_style;
    public:
        std::vector<std::shared_ptr<mev::Gripper>> hands_list;
        std::vector<std::shared_ptr<mev::VisualGeometry>> manipulation_objects;

        ManipulationEnv();
        ~ManipulationEnv();
        void addAxes(const Eigen::Matrix4f& transform,
                     const std::string& x_text,
                     const std::string& y_text,
                     const std::string& z_text);
        bool addHand(const std::string& gripper_urdf_filename, const Eigen::Matrix4f& pose, const std::vector<float> joint_values);
        bool addObject(const std::string& model_filename, const std::string& texture_filename, const Eigen::Matrix4f& pose);
        bool addObject(const std::string& model_filename, const Eigen::Matrix4f& pose);
        void render();
    };
}

#endif